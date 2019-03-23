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
	<br/>
   <li>
    Mencari file dengan ektensi .png pada directory <br>
	  
```c

if(strcmp("_grey.png", &lama[strlen(lama)-9])!=0 && 
strcmp(".png",&lama[strlen(lama)-4])==0)

```
	
  </li>
  
  <li>
	Mengecek apakah ada file yang berekstensi ".png" dan nama filenya tidak diakhiri dengan "_grey.png". Jika ada maka di rename file tersebut menjadi "(namafile)_grey.png". Mengambil nama file tersebut tanpa mengambil ektensi nya lalu di ubah menjadi "(namafile)_grey.png"<br/>

```c

	  	strncpy(baru,lama,strlen(lama)-4);
                strcat(baru,"_grey.png");

```

	  
  </li>
	<br>
  <li>
	  Memindah kan file yang telah direname tadi ke directory /home/[user]/modul2/gambar/. 
	  Variable pwd menyimpan alamat tujuan, lalu variabel baru adalah file yang telah diubah namanya tadi. Dan rename berfungsi juga untuk memindahkan file ke directory tujuan<br/>

```c

	  	 char pwd[] = "/home/arifdarma/modul2/gambar/";
	  	 strcpy(tujuan,pwd);
                 strcat(tujuan,baru);
                 rename(lama,tujuan);

```

   </li>
   <li>
	   Dan menggunakan daemon agar proses selalu berjalan di background jadi program keseluruhannya adalah<br/>

```c
	
	   	int main() {
			pid_t pid, sid;
			char pwd[] = "/home/arifdarma/modul2/gambar/";
			pid = fork();
			if (pid < 0) {
   				exit(EXIT_FAILURE);
  			}
			if (pid > 0) {
				exit(EXIT_SUCCESS);
			}
			umask(0);
			sid = setsid();
			if (sid < 0) {
				exit(EXIT_FAILURE);
			}
			if ((chdir(pwd)) < 0) {
				exit(EXIT_FAILURE);
			}
			close(STDIN_FILENO);
  			close(STDOUT_FILENO);
		  	close(STDERR_FILENO);
  			while(1) {
				DIR* FD;
				struct dirent* in_file;
				char lama[100]={0};
				char baru[100]={0};
				char tujuan[100]={0};
				FD=opendir(".");
				while(in_file=readdir(FD)){
					strcpy(lama,in_file->d_name);
					if(strcmp("_grey.png", &lama[strlen(lama)-9])!=0 && 
					   strcmp(".png",&lama[strlen(lama)-4])==0){
						memset(baru,0,sizeof baru);
						strncpy(baru,lama,strlen(lama)-4);
						strcat(baru,"_grey.png");
						strcpy(tujuan,pwd);
						strcat(tujuan,baru);
						rename(lama,tujuan);
					}
				}
				closedir(FD);
    				sleep(10);
  			}
  			exit(EXIT_SUCCESS);
		}

```

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
   <li>
    Pada parent process, hapus file elen.ku dengan exec<br>
	  
```c

int status;
while((wait(&status))>0);
char *argv[] = {"rm", "hatiku/elen.ku", NULL};
execv("/bin/rm", argv);

```
  </li>
     <li>
    Gunakan daemon process sehingga pengecekan dan penghapusan dapat berjalan otomatis.<br>
    Program untuk soal nomor 2:<br/>
	  
```c

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
	pid_t pid, sid;

	pid = fork();

	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir(".")) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while(1){
		pid_t child_id = fork();
		if(child_id==0){
			pid_t grandchild_id = fork();
			if(grandchild_id<0) exit(EXIT_FAILURE);
			else if(grandchild_id==0){
				sleep(10);
				struct stat buf;
				if(stat("hatiku/elen.ku", &buf)==-1)
					exit(EXIT_FAILURE);
				struct group *gr = getgrgid(buf.st_gid);
				struct  passwd *pw = getpwuid(buf.st_uid);
				if(strcmp(pw->pw_name, "www-data")==0 && strcmp(gr->gr_name, "www-data")==0){
					char *argv[4] = {"chmod", "777", "hatiku/elen.ku", NULL};
					execvp("chmod", argv);
				}
			}
			else{
				int status;
				while((wait(&status))>0);
				char *argv[] = {"rm", "hatiku/elen.ku", NULL};
				execv("/bin/rm", argv);
			}
		}
		else {
			sleep(3);
		}
	}
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

Keseluruhan kode untuk soal nomor 3 : <br/>

```c

// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include <dirent.h>

int main()
{
    pid_t unzip_child = fork();

    if(unzip_child==0) {       // anak
        char *argv[] = {"unzip", "campur2.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }
    else {
        wait(NULL);
        int lstogrep[2], greptowrite[2];
        if(pipe(lstogrep)==-1){
            exit(EXIT_FAILURE);
        }
        if(pipe(greptowrite)==-1){
            exit(EXIT_FAILURE);
        }
        pid_t ls_child = fork();
        if(ls_child==0){ // adiknya anak
            close(greptowrite[0]);
            close(lstogrep[0]);
            close(greptowrite[1]);
            dup2(lstogrep[1], STDOUT_FILENO);
            close(lstogrep[1]);
            close(lstogrep[0]);
            char* argv[] = {"ls", "campur2", NULL};
            execv("/bin/ls", argv);                   
        }
        else{
            pid_t grep_child=fork();
            
            if(grep_child==0){ //child
                close(lstogrep[1]);
                close(greptowrite[0]);
               
                dup2(lstogrep[0], STDIN_FILENO);
                close(lstogrep[0]);
                close(lstogrep[1]);
                dup2(greptowrite[1], STDOUT_FILENO);
                close(greptowrite[1]);
                close(greptowrite[0]);

                char* argv[] = {"grep", "txt\\>", NULL};
                execv("/bin/grep", argv);     
            }
            else{ //parent
                char buff[4096];
                close(lstogrep[1]);
                close(lstogrep[0]);
                close(greptowrite[1]);
                read(greptowrite[0], buff, sizeof(buff));

                close(greptowrite[0]);
                FILE* file = fopen("daftar.txt", "w");
                fprintf(file, "%s", buff);

                fclose(file);
                exit(EXIT_SUCCESS);
            }
        }
    }
}

```
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

<ol>
	<li>
		Kode keseluruhan untuk soal nomor 4 : <br/>
		
```c

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int main() {
  char directory[] = "/home/trash/Documents/makanan/";
  char file_enak[] = "/home/trash/Documents/makanan/makan_enak.txt";

  pid_t daemon, sid;

    int link[2];
    if (pipe(link)==-1)
      exit(EXIT_FAILURE);

	daemon = fork();

	if (daemon < 0) {
		exit(EXIT_FAILURE);
	}

	if (daemon > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir(".")) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

  while(1){
    pid_t sleep_id = fork();
    if(sleep_id<0) exit(EXIT_FAILURE);
    else if(sleep_id>0){
      pid_t pid = fork();
      if (pid<0)
        exit(EXIT_FAILURE);
      else if(pid==0){  // child
        close(link[0]);
        dup2(link[1], STDOUT_FILENO);
        close(link[1]);
        close(link[0]);
        char *argv[] = {"find", file_enak, "-amin", "-0.5", NULL};
        execv("/usr/bin/find", argv);
      }
      else {
        DIR *dir;
        struct dirent *file;
        close(link[1]);
        char buffer[128]="\0", new_file[100];
        int count;
        int n_buff = read(link[0],buffer, sizeof(buffer));
        
        if(n_buff>0){   // kalau pernah dibuka
          dir = opendir(directory);
          count = 0;
          while(file = readdir(dir)){
            if(strstr(file->d_name, "makan_sehat")!=NULL){
              count++;
            }
          }
          close(link[0]);
          close(link[1]);
          sprintf(new_file, "%smakan_sehat%d.txt", directory, ++count);
          char *argv[] = {"touch", new_file, NULL};
          execv("/usr/bin/touch", argv);
        }
      }
    }
    else{
      sleep(5);
    }
  }
 
  return 0;
}

```
</li>
</ol>
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

<ol>
	<li>
		Membuat program utama<br/>
		Kendala : child process tidak terminated sehingga process beranak-pinak.<br>
		
<div>
	
```c

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main() {
  pid_t pid, sid;
  
// ----------------------- membuat daemon
  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
        time_t t;
        time(&t);
        struct tm *waktu = localtime(&t);
        char folder[100];
        char buff[BUFSIZ];
	
// --------------------------------- buat direktori baru setiap setelah meng-copy 30 file (30 menit)
        sprintf(folder, "/home/trash/log/%02d:%02d:%04d-%02d:%02d", waktu->tm_mday, waktu->tm_mon+1, 1900+waktu->tm_year, waktu->tm_hour, waktu->tm_min);
        mkdir(folder, 0777);

// ---------------------------------- meng-copy file syslog dengan nama sesuai iterasi
	int i;
        for(i=1; i<30; i++){
                FILE *log, *dest;
                log = fopen("/var/log/syslog", "r");
                char file[100];
                sprintf(file, "%s/log%d.log", folder, i);
                dest = fopen(file, "w");
                char cr;
                while(fread(buff, 1, BUFSIZ, log))
                {
                    fwrite(buff, 1, BUFSIZ, dest);
                }
                fclose(log);
                fclose(dest);
                sleep(60);
        }
 }
  exit(EXIT_SUCCESS);
}

```

</li>
	<li>
		Membuat program untuk menghentikan program utama menggunakan pkill.
		
```c
		
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    if(pid==0){
        char *argv[] = {"pkill", "soal5", NULL};
        execv("/usr/bin/pkill", argv);
    }
    else{
        exit(EXIT_SUCCESS);
    }
}	

```

</li>
</ol>
<br/>
