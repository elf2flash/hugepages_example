#include <cstdlib>
#include <stdio.h>
#include <sched.h>
//#include <pthread.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include <new>
#include <memory>
#include <utility>

#include <unistd.h>
#include <sys/mman.h>

//#include <sys/mman.h>
//#include <include/linux/mm.h>
//#include <linux/gfp.h>

//#include "huge_pages.h"

#define PAGE_DEFAULT	0
#define PAGE_2MB		21
#define PAGE_1GB		30

#ifndef MAP_HUGE_SHIFT
#define MAP_HUGE_SHIFT 26
#endif

size_t const default_page_size = ::sysconf(_SC_PAGESIZE);

int main()
{
	
	printf("Vhod\n");
	
	/*
	HugePages::warn_no_1GB_pages = advise_hugeadm_1GB;
    HugePages::warn_no_2MB_pages = advise_hugeadm_2MB;
    size_t constexpr MB = 1024 * 1024;
	HugePages hp(HugePages::PAGE_1GB, 32 * MB); // Try allocating a 1GB huge page to minimize TLB misses.
	*/
	
	uint32_t N = 5, Nok = 0;
	uint32_t* p[N];
	uint32_t* ptmp;
	size_t total_size;
	size_t constexpr MB = 1024 * 1024;
	size_t size = 32 * MB;
	
	uint32_t pi = 0, i = 0, ish = 0;
	
	unsigned flags = 0;
	size_t page_size = default_page_size;
	
	page_size = 1u << PAGE_1GB;
	flags = (PAGE_1GB << MAP_HUGE_SHIFT) | MAP_HUGETLB;
	
	total_size = (size + (page_size - 1)) & ~(page_size - 1); // Round up to the page size.
	
	printf("total_size = 0x%x (%d)\n",total_size,total_size);
	printf("flags      = 0x%x (%d)\n",flags,flags);
	
	
	// выделение буферов
	for (pi=0;pi<N;pi++)
	{
		p[pi]  = (uint32_t*)mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED | flags, -1, 0);
		if (p[pi] != MAP_FAILED )
			Nok++;
	}
	// печать адресов буферов
	for (pi=0;pi<N;pi++)
	{
		printf("p[%d]  = 0x%016llx\n", pi, p[pi]);
	}
	
	
	// заполнение буферов нулями
	for (pi=0;pi<Nok;pi++)
	{
		for (i=0;i<total_size/4;i++)
		{
			p[pi][i] = 0;
		}
	}
	
	printf("-----------------------------------------------\n");
	// вывод на экран содержимого буферов
	for (pi=0;pi<Nok;pi++)
	{
		printf("p[%d]:\n",pi);
		for (i=0;i<4;i++)
		{
			printf("0x%08x\t",p[pi][i]);
		}
		printf("\n");
		for (i=total_size/4-4;i<total_size/4;i++)
		{
			printf("0x%08x\t",p[pi][i]);
		}
		printf("\n");
	}
	
	
	
	// заполнение буферов значениями
	for (pi=0;pi<Nok;pi++)
	{
		ish = (pi+1)*0x10000000;
		for (i=0;i<total_size/4;i++)
		{
			p[pi][i] = i + ish;
		}
	}
	
	printf("-----------------------------------------------\n");
	// вывод на экран содержимого буферов
	for (pi=0;pi<Nok;pi++)
	{
		printf("p[%d]:\n",pi);
		for (i=0;i<4;i++)
		{
			printf("0x%08x\t",p[pi][i]);
		}
		printf("\n");
		for (i=total_size/4-4;i<total_size/4;i++)
		{
			printf("0x%08x\t",p[pi][i]);
		}
		printf("\n");
	}
	
	

	printf("Vyhod\n");
	
	
	while (0)
	{
		;
	}
	
	
	int res = 0;
	int Nmunmap = 0;
	for (pi=0;pi<Nok;pi++)
	{
		res = munmap(p[pi], total_size);
		if (res == -1)
			printf("munmap p[%d] fail\n",pi);
		else
			Nmunmap++;
	}
	printf("munmap %d ok\n",Nmunmap);
	
}