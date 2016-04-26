#! /bin/sh

GIT_VERSION_LONG=$(git describe --abbrev=4 --dirty --always --tags)-$(git symbolic-ref --short HEAD)-$(date +%s)
GIT_VERSION_SHORT=$(git describe --abbrev=4 --dirty --always --tags)

if [ -f version.c ]
then
	rm -f version.c

	echo "#include \"version.h\"" >> version.c
	echo
	echo "const char * build_date = \"$(date +%s)\"" >> version.c
