# IPC-Chat


##Kompilacja
Aby skompilowa� potrzebne pliki, nalezy uruchomic skrypt compile.sh.

##Uruchamianie
Poniewa� serwer wymusza stworzenie nowej, niejstniej�cej kolejki, nale�y w pierwszej kolejno�ci go uruchmoi�,
podaj�c jako argument nazw� pliku wej�ciowego:
./sserver data.txt
Aby uruchomi� klienta, nale�y wywo�a� komend�:
./cclient

##Pliki
server.c - serwer
client.c - klient
data.txt - dane wej�ciowe

###Specyfikacja pliku wej�ciowego

W pliku wej�ciowym znajduje si�:
�	9 linijek z nazwami u�ytkownik�w - w i-tej (licz�c od 1) linijce znajduje si� u�ytkownik o ID r�wnym i
�	3 razy po 10 linijek z informacjami o grupach:
	o	1 linijka z nazw� grupy
	o	9 kolejnych linijek z warto�ciami boolowskimi: zawarto�� i-tej linijki opisuje
		przynale�no�� i-tego klienta do danej grupy (1 - u�ytkownik nale�y do grupy, 0 - nie nale�y)

Wszystkie nazwy musz� sk�ada� si� wy��cznie z ma�ych i wielkich liter alfabetu �aci�skiego

##Specyfikacja projektu

Komunikator oparty jest na dw�ch kolejkach IPC: przez jedn� klienci wysy�aj� ��dania i wiadomo�ci do serwera,
a przez drug� - serwer przekazuje wiadomo�ci i wysy�a komunikaty do klient�w.

Ka�dy klient ma przypisane ID [1, 9] ustawiane przez serwer w momencie odczytu danych z pliku wej�ciowego.
Podczas logowania (kt�re polega na podaniu loginu) u�ytkownik wysy�a komunikat "podpisany" w�asnym ID procesu
i w komunikacie zwrotnym otrzymuje sw�j numer. Klient nie zna identyfikator�w innych zalogowanych.

###Menu
Dostepne operacje:
[1]Wyslij wiadomosc do pojedynczego uzytkownika
[2]Wyslij wiadomosc do grupy
[3]Odbierz wiadomosci
[4]Wyswietl liste zalogowanych uzytkownikow
[5]Wyswietl liste grup
[6]Zapisz sie do grupy
[7]Wypisz sie z grupy
[8]Wyswietl liste uzytkownikow nalezacych do grupy
[9]Wyloguj i zamknij

Po pomy�lnym zalogowaniu i zako�czeniu ka�dej innej czynno�ci wy�wietlane jest menu z dost�pnymi operacjami.
Aby wykona� jedn� z nich, nale�y w konsoli poda� jej numer. W przypadku 6, 7 i 8
klient zostanie dodatkowo poproszony o podanie nazwy grupy.

###Rodzaje komunikat�w

struct wiadomosc {
	long id;
	char username[20];
	char text[1000];
};

		Odbierane przez serwer			Odbierane przez klienta
id		ID nadawcy				ID odbiorcy
username	Nazwa adresata (u�ytkownika /grupy)	Nazwa nadawcy
text		Tre�� wiadomo�ci			Tre�� wiadomo�ci

U�ywany, kiedy klient chce skontaktowa� si� z innymi u�ytkownikami.
Zanim zapisze tre�� wiadomo�ci, musi poda� nazw� osoby lub grupy,
z kt�r� zamierza si� skontaktowa�. Klient otrzymuje od serwera zwrotny
komunikat m�wi�cy, czy uda�o si� t� wiadomo�� wys�a� - czyli czy nazwa by�a poprawna.

-------------------------------------------------------------

struct komunikat {
	long type;
	int success_id;
	long user_id;
	char text[500];
};

		Odbierane przez serwer			Odbierane przez klienta
type		Typ komunikatu				ID odbiorcy
success_id	Niewa�ne				1 - sukces, 2 - niepowodzenie
user_id		ID nadawcy				ID klienta (przy logowaniu)
text		Nazwa grupy (kiedy jest potrzebna)	Tre�� komunikatu

Wykorzystywany przez klient�w do zakomunikowania serwerowi ch�ci
podj�cia akcji innej ni� wys�anie lub odebranie wiadomo�ci
oraz przez serwer w reakcji na ka�dy komunikat.
Warto�� numer wskazuje, czy ��danie uda�o si� spe�ni�, a szczeg�y opisuje tekst.

###Typy ��da� (komunikat�w wysy�amych przez klienta do serwera)

Typ	Zadanie						Uwagi
101	Logowanie	
102	Wylogowywanie	
103	Podgl�d aktywnych u�ytkownik�w	
104	Podgl�d u�ytkownik�w nale��cych do grupy	Wymaga podania nazwy grupy
105	Zapisanie do grupy				Wymaga podania nazwy grupy
106	Wypisanie z grupy				Wymaga podania nazwy grupy
107	Wy�wietlenie listy grup	


.
