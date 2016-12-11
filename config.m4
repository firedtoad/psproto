PHP_ARG_WITH(psproto, for psproto support,
[  --with-psproto             Include psproto support])

PHP_ARG_ENABLE(psproto, whether to enable psproto support,
[  --enable-psproto           Enable psproto support])

if test "$PHP_PSPROTO" != "no"; then
  PHP_NEW_EXTENSION(psproto, sproto.c psproto.c, $ext_shared)
fi
