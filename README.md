<h1>LAPORAN SOAL SHIFT MODUL 2</h1>
<div>
  <h3>KELOMPOK E 11</h3>
  <p>05111740000017 Shintya Rezky R.</p>
  <p>05111740000182 Arif Darma A.</p>
</div>

<hr>

<h3>Nomor 1</h3>
<p align="justify">
    Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.<br/>
Catatan : Tidak boleh menggunakan crontab.

</p>
<ol>
  <li>
	 Cek apakah di directory ada file atau directory dengan cara <pre>
	 FD=opendir(".");
                while(in_file=readdir(FD))
	 </pre>
	  Maksud nya ialah cek apakah ditempat program itu berada, apakah ada file didirectory tersebut
  </li>
	<br>
  <li>
	  Mencari file dengan ektensi .png pada directory 
	  <pre>
	  	if(strcmp("_grey.png", &lama[strlen(lama)-9])!=0 && 
		strcmp(".png",&lama[strlen(lama)-4])==0)
	  </pre>
	  Mengecek apakah ada file yang berekstensi ".png" dan nama filenya tidak diakhiri dengan "_grey.png"
  </li>
	<br>
  <li>
	Jika ada maka di rename file tersebut menjadi "(namafile)_grey.png"
	  <pre>
	  	strncpy(baru,lama,strlen(lama)-4);
                strcat(baru,"_grey.png");
	  </pre>
	  Mengambil nama file tersebut tanpa mengambil ektensi nya lalu di ubah menjadi "(namafile)_grey.png"
  </li>
	<br>
  <li>
	  Memindah kan file yang telah direname tadi ke directory /home/[user]/modul2/gambar/
	  <pre>
	  	 char pwd[] = "/home/arifdarma/modul2/gambar/";
	  	 strcpy(tujuan,pwd);
                 strcat(tujuan,baru);
                 rename(lama,tujuan);
	  </pre>
	  Variable pwd menyimpan alamat tujuan, lalu variabel baru adalah file yang telah diubah namanya tadi. Dan rename berfungsi juga untuk memindahkan file ke directory tujuan
   </li>
</ol>
<br/>

<h3>Nomor 2</h3>
<p align="justify">
  Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.<br/>
Catatan: Tidak boleh menggunakan crontab
</p>

<ol>
  <li>
    Buat 2 process menggunakan fork. Parent process untuk menghapus file sedangkan child process untuk mengubah permission.
  </li>
  <li>
    Di child process, cek apakah terdapat file "elen.ku" pada directory "hatiku" dengan group dan owner "www-data". Jika benar, ubah permissionnya menjadi 777 menggunakan chmod.<br>
	  
```c

struct stat buf;
if(stat("hatiku/elen.ku", &buf)==-1)
exit(EXIT_FAILURE);
struct group *gr = getgrgid(buf.st_gid);
struct  passwd *pw = getpwuid(buf.st_uid);
if(strcmp(pw->pw_name, "www-data")==0 && strcmp(gr->gr_name, "www-data")==0){
	char *argv[4] = {"chmod", "777", "hatiku/elen.ku", NULL};
	execvp("chmod", argv);
}

```
  </li>
</ol>
<br/>


<h3>Nomor 3</h3>
<p align="justify">
  Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :<br/>
i)  mengekstrak file zip tersebut.<br/>
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. <br/>
Catatan:  <br/>
Gunakan fork dan exec.<br/>
Gunakan minimal 3 proses yang diakhiri dengan exec.<br/>
Gunakan pipe<br/>
Pastikan file daftar.txt dapat diakses dari text editor<br/>

</p>
<br/>


<h3>Nomor 4</h3>
<p align="justify">
  Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).<br/>
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.<br/>

Contoh:<br/>
File makan_enak.txt terakhir dibuka pada detik ke-1<br/>
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt<br/>

Catatan: <br/>
dilarang menggunakan crontab<br/>
Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst<br/>

</p>
<br/>


<h3>Nomor 5</h3>
<p align="justify">
  Kerjakan poin a dan b di bawah:<br/>
Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log<br/>
Ket:<br/>
Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]<br/>
Per menit memasukkan log#.log ke dalam folder tersebut<br/>
‘#’ : increment per menit. Mulai dari 1<br/>
Buatlah program c untuk menghentikan program di atas.<br/>
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.<br/>

</p>
<br/>
