Mail.ru Agent protocol plugin for Pidgin (libpurple)
====================================================

Copyright 2010-2014 Antonov Nikolay <ostinru@gmail.com>

Copyright 2010-2014 Reslayer <Reslayer@mail.ru>

Copyright 2011-2013 KIV <kiv.apple@gmail.com>

Copyright 2011-2014 Sorokin Alexei <sor.alexei@meowr.ru>

This protocol plugin is based on Nullprpl by Ryan Barrett, and is distributed under the GPLv2+.

Thanks to:

 * Atamurad Hezretkuliyev <atamyrat@gmail.com>
 * Podtykan Maxim <lemax1@mail.ru>

Описание
--------
[mrim-prpl](https://bitbucket.org/mrim-prpl-team/mrim-prpl) – это реализация протокола MMP (Mail.Ru) для Pidgin и libpurple.

Лицензия: GPLv2+

Зависимости:

 * libpurple >= 2.7.0
 * gtk+ >= 2.x
 * также используются cmake, gettext, pkg-config, check

Сборка и установка
------------------
Для некоторых дистрибутивов уже есть готовые пакеты:

1. Debian/Ubuntu пакеты можно взять со странички: https://bitbucket.org/mrim-prpl-team/mrim-prpl/downloads
2. Arch Linux: https://aur.archlinux.org/packages/pidgin-mrim-prpl
3. Gentoo: Добавить оверлей rion и установить mrim-prpl
4. ALT Linux: http://sisyphus.ru/en/srpm/Sisyphus/pidgin-plugin-mrim

Если пакета под Ваш любимый дистрибутив нет, то можно собрать плагин из исходников. Для этого:

1. Для сборки и установки из исходников понадобятся заголовочные файлы libpurple: как их установить зависит от вашего дистрибутива:
    * для Debian и Ubuntu: aptitude install cmake libpurple-dev libgtk2.0-dev check gettext
    * для openSUSE: zypper in cmake libpurple-devel gtk2-devel check-devel gettext
    * для ArchLinux: pacman -S cmake libpurple gtk2 check gettext
    * для RHEL и Fedora: dnf install cmake libpurple-devel gtk2-devel check-devel gettext
2. cmake . && make && sudo make install
3. перезапускаем Pidgin
4. в  "Учётные записи" – "Управление уч. записями" создаём новое подключение к **уже существующему** аккаунту на mail.ru (list.ru, bk.ru…) (то есть зарегистрировать новый аккаунт на сервере mail.ru из Pidgin невозможно).
