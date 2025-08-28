Инструкция по установке и функционалу программы TemperatureStabilization
# Необходимые инструменты
Чтобы установить данную программу понадобится:
- git(инструкции по установке здесь https://git-scm.com/book/ru/v2/%D0%92%D0%B2%D0%B5%D0%B4%D0%B5%D0%BD%D0%B8%D0%B5-%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0-Git)
- Утилита make(установка зависит от вашей ОС, например для Ubuntu https://andreyex.ru/ubuntu/kak-ustanovit-make-na-ubuntu/)
- Библиотека XML-RPC(https://xmlrpc-c.sourceforge.io/downloading.php)
# Установка
Установка утилиты очень простая и включает всего несколько шагов:
- Скопируйте репозиторий командой git clone https://github.com/Coolarsic/C_microcontrollers
- Перейдите в репозиторий командой cd C_microcontrollers
- Вставьте свой токен в константу EXPERIMENT_TOKEN в файле constants.c
- Укажите путь до файлов base.h и client.h библиотеки xmlrpc в файлах dieIfFaultOccured.c, get_thermocouple_signal_mV.c и main.c
- Соберите утилиту из исходников командой make install
- Запустите командой make run 
По умолчанию утилита выводит в консоль три строчки:
- Сигнал с термопары в мВ
- Температуру диода в градусах Цельсия
- Текущее значение тока через элемент Пельтье
Также можно раскомментировать строчки для вывода значений добавок PID-контроллера
# Под капотом


