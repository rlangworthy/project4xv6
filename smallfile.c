
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"


void test_failed(int n){
	printf(1, "TEST %d FAILED\n", n);
}

void test_passed(int n){
	printf(1, "TEST %d PASSED\n", n);	
}


#define MAX 25
#define NBLOCKS (NDIRECT+1)  //12+1
#define SIZE (NBLOCKS*4)   //52


int test1(){
	printf(1,"===Test 1. Trying to create a small file test_file.txt===\n");
	
	char *fileName = "test_file.txt";
	int fd; // Integer for file descriptor returned by open() call 
	fd = open(fileName,O_CREATE | O_SMALLFILE | O_RDWR); 
	// ORDWR - open file for reading and writing
	if (fd==-1){
		printf(1,"Failed to create a small file\n");
		test_failed(1);
		return -1;
	}
	if (fd>0){
		printf(1, "test_file.txt fd is %d\n",fd);
		printf(1, "Opening  smallFileTest.txt\n");
		test_passed(1);
		
	}	
	close(fd);
	return 1;
}

int test2(){
	printf(1,"===Test 2. Trying to write into a small file test_file.txt===\n");
        
    char *fileName = "test_file.txt";
    int fd; // Integer for file descriptor returned by open() call 
 	char buf[SIZE];
  	int n;
  	int i;
  
  	for(i = 0; i < SIZE; i++){
    		buf[i] = (char)(i+(int)'0');
 	 }	
	printf(1, "Size of buffer is %d\n",strlen(buf));

	fd = open(fileName, O_RDWR);
	if (fd>0){
		n = write(fd, buf, MAX);
		if (n==MAX){
			printf(1,"Wrote 25 bytes into a small file\n");
			test_passed(2);
		}else{
			printf(1,"Failed to write 25 bytes into a small file\n");
			test_failed(2);
			}
		close(fd);
		return 1;
	} else {
		printf(1,"Failed to create a small file\n");
		test_failed(2);
		return -1;
	}
}

int test3(){
		printf(1,"===Test 3. Reading test_file.txt after write===\n");
		char *fileName = "test_file.txt";
 		char buf2[SIZE];
		//struct stat st;
		int fd, n; // Integer for file descriptor returned by open() call
		
		if((fd = open(fileName, O_RDWR)) < 0){
   			printf(1, "Failed to open the small file\n");
    		test_failed(3);
			return -1;
  		}
		else{
			printf(1, "Opened the file after write\n");
		}

		n = read(fd, buf2, MAX+10);
		//attempts to read 35 bytes but read 25
		//printf(1, "Number of bytes read : %d\n", n);
		if(n != MAX){
		    printf(1, "Read failed!\n");
    		test_failed(3);
			return -1;
  		}
		else{
			printf(1, "Number of bytes read : %d\n", n);
			printf(1, "String Read : %s\n", buf2);
			test_passed(3);
			close(fd);
			return 1;
		}
}

int test4(){
	printf(1,"===Test 4. fstat returns correctly===\n");
	char *fileName = "test_file.txt";
	struct stat st;
	int fd;

	if((fd = open(fileName, O_RDWR)) < 0){
   		printf(1, "Failed to open the small file\n");
    	test_failed(4);
		return -1;
  	}else{
		printf(1, "Opened test_file.txt\n");
	}
			
	if(fstat(fd, &st) < 0){
   		printf(1, "Failed to get stat on the small file\n");
    	test_failed(4);
		return -1;
 	}else{
		if(st.type != 4 || st.size != MAX){
			printf(1, "Fstat returned incorrect values\n");
			test_failed(4);
			return -1;
		}
		printf(1, "File Type: %d \nFile Size: %d\n", st.type, st.size);
		test_passed(4);
		close(fd);
		return 1;
	 }
}
int test5(){
	printf(1,"===Test 5. Reading and Writing Max size===\n");
	char *fileName = "test_file.txt";
	char buf1[SIZE], buf2[SIZE];
	//struct stat st;
	int fd, n, i; // Integer for file descriptor returned by open() call
	
	for(i = 0; i < SIZE; i++){
    		buf1[i] = (char)(i+(int)'0');
 	 }	
	printf(1, "Size of buffer is %d\n",strlen(buf1));
	
	if((fd = open(fileName, O_RDWR)) < 0){
		printf(1, "Failed to open the small file\n");
		test_failed(5);
		return -1;
	}
	else{
		printf(1, "Opened the file after write\n");
	}
	
	n = write(fd, buf1, SIZE);
	//attempts to write 52 bytes but read 25
	//printf(1, "Number of bytes read : %d\n", n);
	if(n != SIZE){
		printf(1, "Write failed!\n");
		test_failed(5);
		return -1;
	}
	close(fd);
	fd = open(fileName, O_RDWR);
	if((n=read(fd, buf2, SIZE)!=SIZE)){
		printf(1, "Read failed!\n");
		test_failed(5);
		return -1;
	}else{
		for(i = 0; i < SIZE; i++){
    		if(buf1[i] != buf2[i]){
				printf(1, "Read not equal to Write\n");
				test_failed(5);
				return -1;
			}
 	 	}
		test_passed(5);
		close(fd);
		return 1;
	}
}
/*

	if(fstat(fd, &st) < 0){
		printf(1, "Failed to get stat on the small file\n");
			test_failed();
	} else {
		printf(1, "File Type: %i", st.type);
	}

	write(fd, buf2, MAX);
	n=0;
	n=write(fd, buf2, MAX);

	if(n != MAX){
		printf(1, "Write failed!\n");
		test_failed();
	}
	else{
		printf(1, "Number of bytes wrote : %d\n", n);
		printf(1, "String Read : %s\n", buf2);
		test_passed();
		//close(fd);
	}
	if(fstat(fd, &st) < 0){
		printf(1, "Failed to get stat on the small file\n");
			test_failed();
	} else {
		printf(1, "File Type: %d \nFile Size: %d\n", st.type, st.size);
	}


}

*/

int main(){
	printf(1, "==================================\n");
	printf(1, "Test data for small file project\n");	

	if (!(test1()) || 
	    !(test2()) ||
		!(test3()) ||
		!(test4()) ||
		!(test5()))
		printf(1, "Test Failed!\n");
	else
		printf(1, "DONE\n");	

	exit();
}


