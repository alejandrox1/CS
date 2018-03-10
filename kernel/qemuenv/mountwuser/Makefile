setup: 
	./qemu-vfs.sh 

all:  
	qemu-system-x86_64 -kernel ../bzImage -hda qemu-image.img -append "root=/dev/sda single"

clean:
	rm -f qemu-image.img
	sudo umount mount-point.dir && rmdir mount-point.dir
