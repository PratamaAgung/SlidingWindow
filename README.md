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
- `sendToBuffer()` berfungsi untuk memindahkan data dalam vektor ke dalam buffer
- `sendFile()` berfungsi untuk mengirimkan frame-frame yang berada pada buffer
- `receiveACK()` berfungsi untuk mengelola ack yang masuk pada sender
- `timeOutManager()` berfungsi untuk mengelola waktu timeout pengiriman frame dan penerimaan ack
- `sendACK()` berfungsi untuk mengelola pengiriman ack ke sender
- `processMsg()` berfungsi untuk mengelola message yang masuk dan memindahkannya dari buffer
- `waitForMsg()` berfungsi untuk mengelola message yang dikirimkan oleh sender dan dapat mengangani error dan juga pengiriman ack
- `printRecvMsg()` berfungsi untuk mencetak message yang didapat dan menuliskan ke dalam file external

### Pembagian Tugas
| Nama | NIM | Tugas  | 
|:-:|:-:|:-:|
|Pratamamia Agung P.| 13515142  | Fungsi send message frame, fungsi mengelola ack, fungsi timeout    |
|  Irfan Ariq | 13515112  | Fungsi mengelola frame yang masuk pada receiver, fungsi mengirim ack, fungsi mengurutkan frame yang masuk pada receiver |
| Muhamad Irfan M . |  13515037 | Fungsi create socket, fungsi pengelolaan buffer pada sender  |

### Jawaban Pertanyaan
- Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?

    Advertised Window adalah jumlah data yang masih dapat ditampung receiver ke dalam buffer. Jika advertised window bernilai 0 maka pada saat itu buffer di receiver sedang penuh dan tidak dapat untuk menampung data. Oleh karena itu penanganan yang dapat digunakan adalah dengan menunda pengirimannya sampai receiver memberikan advertised window lebih dari 0.

- Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!
![TCP Header](tcp_header.png)

sumber gambar: https://www.lifewire.com/tcp-headers-and-udp-headers-explained-817970
    
    Field data yang terdapat pada TCP Header :
    - Source TCP port number (2 bytes) 
        
    - Destination TCP port number (2 bytes)
 
    - Sequence number (4 bytes)
 
    - Acknowledgment number (4 bytes)
 
    - TCP data offset (4 bits)
 
    - Reserved data (3 bits)
 
    - Control flags (up to 9 bits)
 
    - Window size (2 bytes)
 
    - TCP checksum (2 bytes)
 
    - Urgent pointer (2 bytes)
 
    - TCP optional data (0-40 bytes)
