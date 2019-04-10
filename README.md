# IPC-Chat


##Kompilacja
Aby skompilowaæ potrzebne pliki, nalezy uruchomic skrypt compile.sh.

##Uruchamianie
Poniewa¿ serwer wymusza stworzenie nowej, niejstniej¹cej kolejki, nale¿y w pierwszej kolejnoœci go uruchmoiæ,
podaj¹c jako argument nazwê pliku wejœciowego:
./sserver data.txt
Aby uruchomiæ klienta, nale¿y wywo³aæ komendê:
./cclient

##Pliki
server.c - serwer
client.c - klient
data.txt - dane wejœciowe

###Specyfikacja pliku wejœciowego

W pliku wejœciowym znajduje siê:
•	9 linijek z nazwami u¿ytkowników - w i-tej (licz¹c od 1) linijce znajduje siê u¿ytkownik o ID równym i
•	3 razy po 10 linijek z informacjami o grupach:
	o	1 linijka z nazw¹ grupy
	o	9 kolejnych linijek z wartoœciami boolowskimi: zawartoœæ i-tej linijki opisuje
		przynale¿noœæ i-tego klienta do danej grupy (1 - u¿ytkownik nale¿y do grupy, 0 - nie nale¿y)

Wszystkie nazwy musz¹ sk³adaæ siê wy³¹cznie z ma³ych i wielkich liter alfabetu ³aciñskiego

##Specyfikacja projektu

Komunikator oparty jest na dwóch kolejkach IPC: przez jedn¹ klienci wysy³aj¹ ¿¹dania i wiadomoœci do serwera,
a przez drug¹ - serwer przekazuje wiadomoœci i wysy³a komunikaty do klientów.

Ka¿dy klient ma przypisane ID [1, 9] ustawiane przez serwer w momencie odczytu danych z pliku wejœciowego.
Podczas logowania (które polega na podaniu loginu) u¿ytkownik wysy³a komunikat "podpisany" w³asnym ID procesu
i w komunikacie zwrotnym otrzymuje swój numer. Klient nie zna identyfikatorów innych zalogowanych.

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

Po pomyœlnym zalogowaniu i zakoñczeniu ka¿dej innej czynnoœci wyœwietlane jest menu z dostêpnymi operacjami.
Aby wykonaæ jedn¹ z nich, nale¿y w konsoli podaæ jej numer. W przypadku 6, 7 i 8
klient zostanie dodatkowo poproszony o podanie nazwy grupy.

###Rodzaje komunikatów

struct wiadomosc {
	long id;
	char username[20];
	char text[1000];
};

		Odbierane przez serwer			Odbierane przez klienta
id		ID nadawcy				ID odbiorcy
username	Nazwa adresata (u¿ytkownika /grupy)	Nazwa nadawcy
text		Treœæ wiadomoœci			Treœæ wiadomoœci

U¿ywany, kiedy klient chce skontaktowaæ siê z innymi u¿ytkownikami.
Zanim zapisze treœæ wiadomoœci, musi podaæ nazwê osoby lub grupy,
z któr¹ zamierza siê skontaktowaæ. Klient otrzymuje od serwera zwrotny
komunikat mówi¹cy, czy uda³o siê tê wiadomoœæ wys³aæ - czyli czy nazwa by³a poprawna.

-------------------------------------------------------------

struct komunikat {
	long type;
	int success_id;
	long user_id;
	char text[500];
};

		Odbierane przez serwer			Odbierane przez klienta
type		Typ komunikatu				ID odbiorcy
success_id	Niewa¿ne				1 - sukces, 2 - niepowodzenie
user_id		ID nadawcy				ID klienta (przy logowaniu)
text		Nazwa grupy (kiedy jest potrzebna)	Treœæ komunikatu

Wykorzystywany przez klientów do zakomunikowania serwerowi chêci
podjêcia akcji innej ni¿ wys³anie lub odebranie wiadomoœci
oraz przez serwer w reakcji na ka¿dy komunikat.
Wartoœæ numer wskazuje, czy ¿¹danie uda³o siê spe³niæ, a szczegó³y opisuje tekst.

###Typy ¿¹dañ (komunikatów wysy³amych przez klienta do serwera)

Typ	Zadanie						Uwagi
101	Logowanie	
102	Wylogowywanie	
103	Podgl¹d aktywnych u¿ytkowników	
104	Podgl¹d u¿ytkowników nale¿¹cych do grupy	Wymaga podania nazwy grupy
105	Zapisanie do grupy				Wymaga podania nazwy grupy
106	Wypisanie z grupy				Wymaga podania nazwy grupy
107	Wyœwietlenie listy grup	


.
