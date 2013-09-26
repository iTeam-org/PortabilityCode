#!/bin/sh -e

tmp="/tmp/tmp2LqYfht9Ee"

make doc
zip -r portability.zip portability.c portability.h
mv portability.zip /tmp/portability.zip
cp -R doc/html $tmp
git checkout gh-pages
rm -Rf doc
cp -R $tmp doc
mv /tmp/portability.zip .
touch .nojekyll
git add .
git commit -am "Automatic documentation upload"
git push origin gh-pages
git checkout master

exit 0

