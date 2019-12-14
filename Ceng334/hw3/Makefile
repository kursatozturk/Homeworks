EXEC:=filecopy
IMAGE:=image.img
BS:=1024
COUNT:=128
INODE_COUNT:=32
SF = img.jpg
REMOTE_HOST:=divan.ceng.metu.edu.tr
REMOTE_USER:=e2171874
PORT:=8085
REMOTE_DIR:=/home/bs05/e2171874/ceng334

clearImg:
	rm -rf $(IMAGE)
	dd if=/dev/zero of=${IMAGE} bs=${BS} count=${COUNT}
createImg: clearImg
	mke2fs -N ${INODE_COUNT} $(IMAGE)

all: main.cpp
	g++ -std=c++14 new_main.cpp -o ${EXEC}
clean:
	rm -rf ${EXEC}
debug: clean all
	./${EXEC} ${IMAGE} ${SF} ${target}

check:
	e2fsck -c ${IMAGE}


sendimage: 
	scp -P ${PORT} ${IMAGE} ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_DIR}/${IMAGE}
inek: sendimage
	echo ${REMOTE_DIR}
	ssh -p ${PORT} ${REMOTE_USER}@${REMOTE_HOST}
