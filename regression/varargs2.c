#include <stdarg.h>
#include <stdio.h>

void minivprintf(const char * fmt, va_list ap)
{
  char c;

  while (1) {
    switch(c = *fmt++) {
    case 0:
      return;
    case '%':
      switch (*fmt++) {
      case '%':
        putchar('%');
        break;
      case 'c':
        printf("%c", (char) va_arg(ap, int));
        break;
      case 's':
        printf("%s", va_arg(ap, char *));
        break;
      case 'd':
        printf("%d", va_arg(ap, int));
        break;
      case 'l':
        printf("%lld", va_arg(ap, long long));
        break;
      case 'e':
        printf("%.10g", va_arg(ap, double));
        break;
      case 'f':
        printf("%.10g", (float) va_arg(ap, double));
        break;
      default:
        puts("<bad format>");
        return;
      }
      break;
    default:
      putchar(c);
      break;
    }
  }
}

void miniprintf(const char * fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  minivprintf(fmt, va);
  va_end(va);
}

/* Run va_start twice */

void miniprintf2(const char * fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  minivprintf(fmt, va);
  va_end(va);
  va_start(va, fmt);
  minivprintf(fmt, va);
  va_end(va);
}

/* Use va_copy */

void miniprintf3(const char * fmt, ...)
{
  va_list va, va2;
  va_start(va, fmt);
  va_copy(va2, va);
  minivprintf(fmt, va);
  minivprintf(fmt, va2);
  va_end(va);
  va_end(va2);
}

/* Add some dummy arguments to force overflow into stack-passed params
   (mostly relevant for ARM and PowerPC) */

void miniprintf_extra(int i1, int i2, int i3, int i4, 
                      int i5, int i6, int i7, int i8,
                      double f1, double f2, double f3, double f4,
                      float f5, float f6, float f7, float f8,
                      const char * fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  minivprintf(fmt, va);
  va_end(va);
}

/* Test va_list compatibility with the C library */

void printf_compat(const char * fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  va_end(va);
}

/* The test harness */

int main()
{
  miniprintf("An int: %d\n", 42);
  miniprintf("A long long: %l\n", 123456789012345LL);
  miniprintf("A string: %s\n", "Hello world");
  miniprintf("A double: %e\n", 3.141592654);
  miniprintf("A char: %c and "
             "A string: %s and "
             "An int: %d and "
             "A long: %l and "
             "A double: %e and "
             "A float: %f\n",
             'x',
             "Hello, world!",
             42,
             123456789012345LL,
             3.141592654,
             2.71828182);
  miniprintf2("Twice: %d %e\n", -1, 1.23);
  miniprintf3("With va_copy: %d %e\n", -1, 1.23);
  miniprintf_extra(0, 1, 2, 3, 4, 5, 6, 7,
                   0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
             "With extra args: "
             "A char: %c and "
             "A string: %s and "
             "An int: %d and "
             "A long: %l and "
             "A double: %e and "
             "A float: %f\n",
             'x',
             "Hello, world!",
             42,
             123456789012345LL,
             3.141592654,
             2.71828182);
  printf_compat("va_list compatibility: "  
                "A char: %c and "
                "A string: %s and "
                "An int: %d and "
                "A long: %lld and "
                "A double: %.10g and "
                "A float: %.10g\n",
                'x',
                "Hello, world!",
                42,
                123456789012345LL,
                3.141592654,
                (float) 2.71828182);
  return 0;
}




