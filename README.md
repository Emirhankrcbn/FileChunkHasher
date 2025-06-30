# Dosya Parçalama ve Hash Hesaplama (C++)

Bu C++ uygulaması, verilen bir dosyayı 1MB'lık parçalara ayırır ve her parçanın **MD5**, **SHA256** ve **SHA512** hash'ini hesaplar.

## 🧩 Özellikler
- Dosya parçalayıcı (1MB'lık chunk sistemi)
- OpenSSL kullanarak çoklu hash algoritması
- Hash sonuçlarını `hash_results.txt` dosyasına yazar

## 🛠 Gereksinimler
- C++14 destekli derleyici (Visual Studio 2022)
- OpenSSL kütüphanesi

## 🚀 Kullanım
1. Visual Studio'da projeyi açın
2. Parçalanacak dosya adını girin (örneğin `ornek.txt`)
3. Parçalar (`chunk_0.bin`, `chunk_1.bin`, ...) oluşturulur
4. `hash_results.txt` içinde her parçanın hash'leri yazılı olur

## 🧾 Lisans
MIT Lisansı
