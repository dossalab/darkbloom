static size_t utf8len(const void *str)
{
	const unsigned char *s = (const unsigned char *)str;
	size_t length = 0;

	while ('\0' != *s) {
		if (0xf0 == (0xf8 & *s)) {
			// 4-byte utf8 code point (began with 0b11110xxx)
			s += 4;
		} else if (0xe0 == (0xf0 & *s)) {
			// 3-byte utf8 code point (began with 0b1110xxxx)
			s += 3;
		} else if (0xc0 == (0xe0 & *s)) {
			// 2-byte utf8 code point (began with 0b110xxxxx)
			s += 2;
		} else { // if (0x00 == (0x80 & *s)) {
			// 1-byte ascii (began with 0b0xxxxxxx)
			s += 1;
		}

		// no matter the bytes we marched s forward by, it was
		// only 1 utf8 codepoint
		length++;
	}

	return length;
}
