#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>


//static char receive[];

char * message, *receive;
int testcount = 0; // needed to be used globally
//EXPORT_SYMBOL(testcount);

int countUCF(){
	int i = 0, output = 0;
	// Need to be careful here - easy to get an array out of bounds with current implementation
	for(i=0; i < strlen(message); i++){
		if(i < strlen(message)-2){
			if(message[i] == 'U' && message[i+1] == 'C' && message[i+2] == 'F'){
					output++;
			}
		}
	}
	return output;
}

int main(){
	
	
	int i = 0;
	int total;
	message = (char *)malloc(sizeof(char)*256);
	
	for(i = 0; i < strlen(message); i++){
		message[i] = '0';
	}
	
	int ret, fd, fd2;
	
	printf("Starting device test code example...\n");
	
	fd = open("/dev/HW4Read", O_RDWR);
	fd2 = open("/dev/HW4Write", O_RDWR);
	
	if(fd < 0 || fd2 < 0){
		perror("Failed to open the device...");
		return errno;	
	}
	
	printf("Opened Kernel Module Character Driver\n");
	printf("Type in a short string to send to the kernel module:\n");
	
	scanf("%[^\n]%*c", message);
	
	printf("Writing message to the device [%s].\n", message);
	
	testcount = countUCF();
	total = testcount * 35 + strlen(message);
	
	ret = write(fd2, message, total);
	
	if(ret < 0){
		perror("Failed to write the message to the device.");
		return errno;	
	}
	
	printf("Press ENTER to read back from the device...\n");
	
	getchar();
	
	printf("Reading from the device...\n");
	
	receive = (char *)malloc(sizeof(char)*total);
	
	// Allocate receive memory
	
	ret = read(fd, receive, total);
	
	if(ret < 0){
		perror("Failed to read the message from the device.");
		return errno;	
	}
	
	printf("The received message is: [%s]\n", receive);
	printf("End of the program\n");
	
	free(message);
	free(receive);
	
	close(fd);
	close(fd2);
	
	return 0;
	
}
