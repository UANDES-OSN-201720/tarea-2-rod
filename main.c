/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int *frames;
char algo[8];
void page_fault_handler( struct page_table *pt, int page )
{
	//aqui toca hacer el codigo, para empezar, hay que setear segun fifo y random la memoria dependiendo de cual es cual
	//lo cual lo haremos haciendo uso de una variable global, yei vivan los paradigmas de programacion
	//luego hay que verificar si la pagina esta cargada en memoria, si no en disco tocara
	
	int marc = page_table_get_npages(pt);
	//char **marco;
	//marco = (char**)malloc(sizeof(char *)*marc);

	//for (int i =0; i < marc; i++){
		//marco[i] = (char *)malloc(sizeof(char)*PAGE_SIZE);//crear una tabla con las direcciones de los marcos, puede ser que este mal
	//}

	//page_table_set_entry(pt, 1, 1, PROT_WRITE);
	//char *virt = page_table_get_virtmem(pt);
	//char *fisi = page_table_get_physmem(pt);
	if(!(strcmp("rand", algo))){
		//aqui va la ordenacion random
		//hay que cer si los la pagina esta en los marcos, si no hay que fetchearla de disco
		char *virt = page_table_get_virtmem(pt);
		char *fisi = page_table_get_physmem(pt);
		if (page_table_get_nframes(pt) < page_table_get_npages(pt)){//check si hay menos frames que paginas, por lo toc ahacer swap
			//al parecer no entiendo bien el programa NO EJECUTAR!!! A MI ME CONSUMIO TODA LA MEMORIA Y TOCA REINICIAR
		}
	}
	
	
}

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		/* Add 'random' replacement algorithm if the size of your group is 3 */
		printf("use: virtmem <npages> <nframes> <lru|fifo> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];
	strcpy(algo, argv[3]);
	frames = (malloc(sizeof(char)*nframes));
	
	for (int i =0; i < nframes; i++){
		frames[i] = 0;//llena los espacios de marcos con cero, osea libre
	} 
	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}


	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
