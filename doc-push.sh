#!/bin/sh

set u
set e

tmp="/tmp/CHljfdfgUGF43Ddl"
make doc
cp -R html $tmp
git checkout gh-pages
rm -Rf doc
cp -R $tmp doc
touch .nojekyll
git add .
git commit -am "Automatic documentation push"
git push origin gh-pages
git checkout master

exit 0

