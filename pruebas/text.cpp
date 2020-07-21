/* $OpenBSD: text.c,v 1.3 2017/04/18 14:16:48 nicm Exp $ */

/*
 * Copyright (c) 2015 Nicholas Marriott <nicm@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <iostream>

static int text_is_ascii(u_char c)
{
	const char	cc[] = "\007\010\011\012\014\015\033";

	if (c == '\0')
		return (0);
	if (strchr(cc, c) != NULL)
		return (1);
	return (c > 31 && c < 127);
}

static int text_is_latin1(u_char c)
{
	if (c >= 160)
		return (1);
	return (text_is_ascii(c));
}

static int text_is_extended(u_char c)
{
	if (c >= 128)
		return (1);
	return (text_is_ascii(c));
}

static int
text_try_test(const std::string data, int (*f)(u_char))
{
    for (char c: data)
    {
		if (!f(c)) return (0);
    }
	return (1);
}

const char *
text_get_type(const std::string base)
{
	if (text_try_test(base, text_is_ascii))
		return ("ASCII");
	if (text_try_test(base, text_is_latin1))
		return ("ISO-8859");
	if (text_try_test(base, text_is_extended))
		return ("Non-ISO extended-ASCII");
	return (NULL);
}

