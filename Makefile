.PHONY: pdf epub web clean

pdf:
	docker compose run --rm review review-pdfmaker config.yml

epub:
	docker compose run --rm review review-epubmaker config.yml

web:
	docker compose run --rm review review-webmaker config.yml

text:
	docker compose run --rm review review-textmaker config.yml

clean:
	rm -f *.pdf *.epub
	rm -rf webroot
