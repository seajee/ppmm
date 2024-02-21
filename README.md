# ppmm

Image manipulation using the Portable PixMap format.

## Quick start

To quickly build the project you can simply run make

```bash
$ make
```

## Examples

### Sample images

![cat](assets/cat.jpg)
![cat grayscale](assets/cat_bw.jpg)
![cat inverted](assets/cat_inv.jpg)
![cat color](assets/cat_col.jpg)

### Filters

```bash
$ ./ppmm g cat.ppm cat_bw.ppm  # grayscale
$ ./ppmm i cat.ppm cat_inv.ppm # inverted
$ ./ppmm m cat.ppm cat_mul.ppm # multiply by value
$ ./ppmm n cat.ppm cat_col.ppm # multiply by color
```

### Compression

```bash
$ ./rle > blank.ppm                        # generate blank image
$ ./ppmm c blank.ppm blank_comp.ppm        # compress image with RLE
$ ./ppmm e blank_comp.ppm blank_decomp.ppm # decompress image
```
