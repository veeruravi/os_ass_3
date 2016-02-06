#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>

key_t key = IPC_PRIVATE;

void selectionSort(int arr[],int low,int high,int n);
void mergeSort(int arr[],int low,int mid,int high,int n);
void partition(int arr[],int low,int high,int n);
int main()
{
	int i,n;
	scanf("%d",&n);
	int *merge,shmid;
	if((shmid=shmget(key, sizeof(int) *n, IPC_CREAT| 0666)) < 0)
	{
		perror("Shmget failed");
		exit(1);
	}
	if((merge = shmat(shmid, 0, 0)) == (int *) -1)
	{
		perror("Shmat failed");
		exit(1);
	}
	for(i=0;i<n;i++)
		scanf("%d",&merge[i]);
	partition(merge,0,n-1,n);
	for(i=0;i<n-1;i++)
		printf("%d ",merge[i]);
	printf("%d\n",merge[i]);
	return 0;
}
void partition(int arr[],int low,int high,int n)
{
	int mid,status;
	if(high-low<=4)
		selectionSort(arr,low,high,n);
	else
	{
		mid=(low+high)/2;
		pid_t pid1,pid2;
		if((pid1=fork())<0)
		{
			perror("fork Failed");
			exit(1);
		}
		if (pid1==0)
		{
			partition(arr,low,mid,n);
			exit(0);
		}
		waitpid(pid1, &status, 0);
		if((pid2=fork())<0)
		{
			perror("fork Failed");
			exit(1);
		}
		if(pid2==0)
		{
			partition(arr,mid+1,high,n);
			exit(0);
		}
		waitpid(pid2, &status, 0);
		mergeSort(arr,low,mid,high,n);
	}
}
void selectionSort(int arr[],int low,int high,int n)
{
	int i,min,i1,j,temp;
	for (i = low; i <= high; i++)
	{
		min = arr[i];
		for (j = i; j <= high; j++)
			if (arr[j]<min)
			{
				i1=j;
				min=arr[j];
			}
		if(min!=arr[i])
		{
			temp = arr[i1];
			arr[i1] = arr[i];
			arr[i] = temp;
		}
	}
}
void mergeSort(int arr[],int low,int mid,int high,int n)
{

	int i,m,k,l,temp[n];

	l=low;
	i=low;
	m=mid+1;

	while((l<=mid)&&(m<=high)){

		if(arr[l]<=arr[m]){
			temp[i]=arr[l];
			l++;
		}
		else{
			temp[i]=arr[m];
			m++;
		}
		i++;
	}

	if(l>mid){
		for(k=m;k<=high;k++){
			temp[i]=arr[k];
			i++;
		}
	}
	else{
		for(k=l;k<=mid;k++){
			temp[i]=arr[k];
			i++;
		}
	}

	for(k=low;k<=high;k++){
		arr[k]=temp[k];
	}
}
