/**
 * MIT License
 * Copyright (c) 2020 Mitosis-Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///< this is the cacheline size
#define CONFIG_CACHELINE_SIZE 64

///< this is the large page size alignment
#define CONFIG_LARGE_PAGE_SIZE (1 << 21)

///< define this if you have a modified kernel with Mitosis support
#define CONFIG_HAVE_MODIFIED_KERNEL 1

///< the name of the shared memory file created
#define CONFIG_SHM_FILE_NAME "/tmp/mitosis-btree-bench"


#define CONFIG_DEFAULT_NELEMENTS   (1UL << 20)

///< the number of lookups
#define CONFIG_DEFAULT_NLOOKUP 10000000UL

///< the default order of the tree
#define CONFIG_DEFAULT_ORDER 6

///< the default key stride
#define CONFIG_DEFAULT_KEY_STRIDE 2

///< the size of the tuple in quad words
#define CONFIG_ELEMENT_TUPLE_SIZE 15
