#!/bin/sh

if git diff ckw | grep dirty > /dev/null 2>&1; then
  echo
else
  (cd ckw; git apply ../patches/01-option.diff)
fi

if [ ! -d gtest ]; then
  git svn clone -r HEAD http://googletest.googlecode.com/svn/tags/release-1.6.0/ gtest
fi
