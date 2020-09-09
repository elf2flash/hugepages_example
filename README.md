# hugepages_example
First project (example) using hugepage 1G.
Centos Linux 3.10.0-1127.el7.x86_64.

1. Backup files to 
/home/user/grub_backup/etc_default_grub    - grub
/home/user/grub_backup/boot_efi_EFI_centos - grub.cfg

2. Add to /etc/default/grub parameters:
hugepagesz=1g hugepages=64 default_hugepagesz=1g

3. grub2-mkconfig -o /boot/efi/EFI/centos/grub.cfg

4. Reboot

5. dmesg logs thet allocate 13 pages:
HugeTLB: allocating 64 of page size 1 GB failed.  Only allocated 13 hugepages.
