# Sliding Window Project
## Group ::`HaveFun`:: K-01
### IF3130 Jaringan Komputer, Institut Teknologi Bandung, 2017
---------------------------------------------------------------
### Petunjuk Penggunaan
Program simulasi protokol flow control `Sliding Window` ini dikembangkan dalam bahasa C++. Terdapat 5 file utama yang dikembangkan dalam project ini, yaitu `sendfile.cpp`, `receiver.cpp`, `sendframe.h`, `frameack.h`, dan `windowstatus.h`. TElah disediakan makefile untuk mempermudah dalam melakukan kompilasi dan menjalankan program.
Untuk melakkan kompilasi program, dapat dilakukan dengan mengetik command
```
make build
```
Kemudian, untuk menjalankan program receiver dilakukan dengan mengetik command
```
make run-receiver
```
Untuk menjalankan program sendfile dilakukan command berikut
```
make run-sendfile
```

### Cara Kerja Sliding Window
Pada program yang dibuat, terdapat buffer yang akan menyimpan data siap kirim pada sender dan data telah diterima pada receiver. Sliding window diimplementasikan sebagai suatu interval pada buffer yang dibatasi oleh dua integer sebagai indeks, yaitu `lowerWindow` dan `upperWindow`. Selain itu, untuk mengontrol status terkirim tidaknya suatu paket data, digunakan suatu kelas bernama `WindowStatus`. Pada kelas ini terdapat data status terkirimnya paket (telah terkirim atau belum terkirim) dan waktu terakhir paket dikirim. Waktu terakhir ini akan berguna dalam memberikan status timeout pada paket data. Jika paket data telah timeout, maka paket itu akan dikirim kembali. Pada receiver, setiap menerima suatu paket data, maka receiver akan mengembalikan ACK kepada sender. ACK ini berisi sequence number dari data berikutnya yang diharapkan oleh receiver. Sender dan receiver akan menyesuaikan window-nya dengan data yang sedang dipertukarkan. Hal ini akan berlangsung terus hingga frame terakhir dikirim. Saat ACK dari frame terakhir diterima oleh sender, maka sender akan mengirim frame dengan data berisi NULL dan dengan sequence number yang menandakan bahwa proses transmisi data telah selesai.
Fungsi-fungsi yang terlibat dalam proses transimsi data penggunakan flow control protocol Sliding Window adalah sebagai berikut:
- `fillBuffer()` berfungsi sebagai pembaca file kemudian memasukkannya ke dalam vector
- `sendToBuffer()`
- 
### Pembagian Tugas
| Nama | NIM | Tugas  | 
|:-:|:-:|:-:|
|Pratamamia Agung P.| 13515142  | Fungsi send message frame, fungsi mengelola ack, fungsi timeout    |
|  Irfan Ariq | 13515112  | Fungsi mengelola frame yang masuk pada receiver, fungsi mengirim ack, fungsi mengurutkan frame yang masuk pada receiver |
| Muhamad Irfan M . |  13515037 | Fungsi create socket, fungsi pengelolaan buffer pada sender  |

### Jawaban Pertanyaan
- Apakah yang akan terjadi jika advertised window size bernilai 0?
- Field data yang  terdapat pada TCP header adalah sebagai berikut