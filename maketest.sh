#!/bin/bash
if make debug test clean
	then ./tests/utest
fi
