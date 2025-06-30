#include <iostream>
#include <fstream> //dosya okuma yazma
#include <string>
#include <iomanip> //hash değerlerini hex string'e çevirmek için   
#include <sstream> //hash değerini string yap
#include <filesystem> //dosya yolu işlemleri
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <clocale>  //türkçe karakter

using namespace std;

void dosyaBöl(const string& DosyaAdı, size_t Size = 1024 * 1024) 
{
    ifstream inputFile(DosyaAdı, ios::binary); //dosyayı binary olarak açar
    if (!inputFile) 
    {
        cerr << "HATA: Dosya açılamadı: " << DosyaAdı << endl;
        return;
    }

    char* buffer = new char[Size];
    int chunkNo = 0;

    while (!inputFile.eof()) //dosyayı her seferinde 1mb oku
    {
        inputFile.read(buffer, Size);
        streamsize bytesRead = inputFile.gcount();

        if (bytesRead <= 0) break; //son veri okunduğunda çık

        string outputFileName = "chunk_" + to_string(chunkNo) + ".bin"; //parça dosyaları üret
        ofstream outputFile(outputFileName, ios::binary);

        if (!outputFile) 
        {
            cerr << "HATA: Parça dosyası yazılamadı: " << outputFileName << endl;
            break;
        }

        outputFile.write(buffer, bytesRead);
        outputFile.close();


        cout << outputFileName << " yazıldı. (" << bytesRead << " bayt)" << endl;
        chunkNo++;
    }

    delete[] buffer;
    inputFile.close();
    cout << "Toplam " << chunkNo << " parça oluşturuldu.\n" << endl;
}

//binary verileri okunabilir yap (hex e çevir)
string bytesToHex(const unsigned char* hash, int length) 
{
    ostringstream oss;
    for (int i = 0; i < length; ++i) 
    {
        oss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return oss.str();
}

void hashFile(const string& dosyaYolu,string& md5, string& sha256, string& sha512) 
{
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    ifstream file(dosyaYolu, ios::binary);
    
    //geçici veri tutar
    MD5_CTX md5Ctx;
    SHA256_CTX sha256Ctx;
    SHA512_CTX sha512Ctx;

	//algoritmaları başlat
    MD5_Init(&md5Ctx);
    SHA256_Init(&sha256Ctx);
    SHA512_Init(&sha512Ctx);

    while (file.good()) 
    {
        file.read(buffer, BUFFER_SIZE);//bellek verimliliği
		streamsize okunanVeri = file.gcount();//okunan bayt sayısı
        if (okunanVeri > 0) { //değerleri güncelle
            MD5_Update(&md5Ctx, buffer, okunanVeri);
            SHA256_Update(&sha256Ctx, buffer, okunanVeri);
            SHA512_Update(&sha512Ctx, buffer, okunanVeri);
        }
    }
    //hash sonuçlarını sakla
    unsigned char md5Result[MD5_DIGEST_LENGTH];
    unsigned char sha256Result[SHA256_DIGEST_LENGTH];
    unsigned char sha512Result[SHA512_DIGEST_LENGTH];

    //algoritma bitti (son hash değeri)
    MD5_Final(md5Result, &md5Ctx);
    SHA256_Final(sha256Result, &sha256Ctx);
    SHA512_Final(sha512Result, &sha512Ctx);

    md5 = bytesToHex(md5Result, MD5_DIGEST_LENGTH);
    sha256 = bytesToHex(sha256Result, SHA256_DIGEST_LENGTH);
    sha512 = bytesToHex(sha512Result, SHA512_DIGEST_LENGTH);

    file.close();
}

// Parçalanmış dosyaların hash değerlerini hesapla ve yaz
void ChunkDosyaları(const string& outputFileName = "hash_results.txt") 
{
    ofstream out(outputFileName);
    if (!out) 
    {
        cerr << "HATA: " << outputFileName << " yazılamadı.\n";
        return;
    }

    int chunkNo = 0;
    while (true) 
    {
        string chunkName = "chunk_" + to_string(chunkNo) + ".bin";
        if (!filesystem::exists(chunkName)) break;

        string md5, sha256, sha512;
        hashFile(chunkName, md5, sha256, sha512);

        out << chunkName << ":\n";
        out << "  MD5    : " << md5 << "\n";
        out << "  SHA256 : " << sha256 << "\n";
        out << "  SHA512 : " << sha512 << "\n\n";

        cout << chunkName << " için hash hesaplandı.\n";
        chunkNo++;
    }

    out.close();
    cout << "\nTüm hash değerleri " << outputFileName << " dosyasına yazıldı.\n";
}

int main() 
{
	setlocale(LC_ALL, "Turkish");

    string fileName;
    cout << "Parçalanacak dosyanın adını (veya yolunu) girin: ";
    getline(cin, fileName);

    dosyaBöl(fileName);         // 1MB'lık parçalar
    ChunkDosyaları();       // Her parça için dosya oluştur

    return 0;
}
