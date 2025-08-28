# Простой Makefile для компиляции C проекта

# Шаг 1: Компилируем все исходные файлы в объектные
constants.o: constants.c constants.h
	gcc -Wall -Wextra -std=c17 -g -c constants.c -o constants.o

dieIfFaultOccured.o: dieIfFaultOccured.c dieIfFaultOccured.h
	gcc -Wall -Wextra -std=c17 -g -c dieIfFaultOccured.c -o dieIfFaultOccured.o

get_thermocouple_signal_mV.o: get_thermocouple_signal_mV.c get_thermocouple_signal_mV.h constants.h dieIfFaultOccured.h
	gcc -Wall -Wextra -std=c17 -g -c get_thermocouple_signal_mV.c -o get_thermocouple_signal_mV.o

emf_to_temperature.o: emf_to_temperature.c emf_to_temperature.h constants.h
	gcc -Wall -Wextra -std=c17 -g -c emf_to_temperature.c -o emf_to_temperature.o

main.o: main.c constants.h dieIfFaultOccured.h get_thermocouple_signal_mV.h emf_to_temperature.h
	gcc -Wall -Wextra -std=c17 -g -c main.c -o main.o

# Шаг 2: Линкуем все объектные файлы в исполняемый
install: main.o constants.o dieIfFaultOccured.o get_thermocouple_signal_mV.o emf_to_temperature.o
	gcc -g -o thermocouple_app main.o constants.o dieIfFaultOccured.o get_thermocouple_signal_mV.o emf_to_temperature.o -lm -ldl -lxmlrpc -lxmlrpc_client -lxmlrpc_server -lxmlrpc_util 

# Шаг 3: Очистка
clean:
	rm -f *.o thermocouple_app

# Шаг 4: Пересборка
rebuild: clean thermocouple_app

# Шаг 5: Запуск
run: thermocouple_app
	./thermocouple_app