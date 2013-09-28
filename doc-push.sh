#!/bin/sh -e

tmp="/tmp/tmp2LqYfht9Ee"
p=portability

clean() {
   rm -Rf ${tmp}
}

trap clean EXIT

clean


make clean
make docu

zip -r ${p}.zip ${p}.c ${p}.h
mv ${p}.zip /tmp/${p}.zip

mv html $tmp

git checkout gh-pages

rm -Rf doc
mv $tmp doc
mv /tmp/${p}.zip .
touch .nojekyll

git add .
git commit -am "Automatic documentation upload"
git push origin gh-pages

git checkout master

clean

exit 0

