.PHONY: clean

%_drom.png: %_drom.svg
	inkscape --export-background-opacity=0 --export-dpi=300 $< -o $@

%_drom.svg: %_drom.json drom_convert.py
	python drom_convert.py $< $@

clean:
	rm -f *_drom.svg *_drom.png