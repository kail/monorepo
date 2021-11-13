HUGO?=hugo

BASEDIR=$(CURDIR)
OUTPUTDIR=$(BASEDIR)/public

help:
	@echo 'Makefile for a Hugo site                                                                      				'
	@echo '                                                                                                     '
	@echo 'Usage:                                                                                               '
	@echo '   make help               output this message                                                       '
	@echo '   make clean              remove the generated files                                                '
	@echo '   make debug              serve site at http://localhost:1313 with automatic reloading after changes'
	@echo '   make build              generate the static site					                                        '
	@echo '   make upload             upload the web site via S3     					    															'
	@echo '                                                                                                     '

clean:
	[ ! -d $(OUTPUTDIR) ] || rm -rf $(OUTPUTDIR)

debug:
	$(HUGO) server -D

build:
	$(HUGO) -D

upload: build
	AWS_PROFILE=skobovm $(HUGO) deploy

.PHONY: help clean debug build upload
