# IPC-Chat


## Kompilacja
Aby skompilować potrzebne pliki, nalezy uruchomic skrypt compile.sh.

## Uruchamianie
Ponieważ serwer wymusza stworzenie nowej, niejstniejącej kolejki, należy w pierwszej kolejności go uruchmoić,
podając jako argument nazwę pliku wejściowego:<br/>
./sserver data.txt<br/>
Aby uruchomić klienta, należy wywołać komendę:<br/>
./cclient

## Pliki

server.c - serwer<br/>
client.c - klient<br/>
data.txt - dane wejściowe<br/>

### Specyfikacja pliku wejściowego

<ul>W pliku wejściowym znajduje się:
<li>9 linijek z nazwami użytkowników - w i-tej (licząc od 1) linijce znajduje się użytkownik o ID równym i</li>
<li><ul>	3 razy po 10 linijek z informacjami o grupach:
	<li>	1 linijka z nazwą grupy</li>
	<li>	9 kolejnych linijek z wartościami boolowskimi: zawartość i-tej linijki opisuje
		przynależność i-tego klienta do danej grupy (1 - użytkownik należy do grupy, 0 - nie należy)</li></ul></li>
	</ul>

Wszystkie nazwy muszą składać się wyłącznie z małych i wielkich liter alfabetu łacińskiego

## Specyfikacja projektu

Komunikator oparty jest na dwóch kolejkach IPC: przez jedną klienci wysyłają żądania i wiadomości do serwera,
a przez drugą - serwer przekazuje wiadomości i wysyła komunikaty do klientów.

Każdy klient ma przypisane ID [1, 9] ustawiane przez serwer w momencie odczytu danych z pliku wejściowego.
Podczas logowania (które polega na podaniu loginu) użytkownik wysyła komunikat "podpisany" własnym ID procesu
i w komunikacie zwrotnym otrzymuje swój numer. Klient nie zna identyfikatorów innych zalogowanych.

### Menu
Dostepne operacje:<br/>
[1]Wyslij wiadomosc do pojedynczego uzytkownika<br/>
[2]Wyslij wiadomosc do grupy<br/>
[3]Odbierz wiadomosci<br/>
[4]Wyswietl liste zalogowanych uzytkownikow<br/>
[5]Wyswietl liste grup<br/>
[6]Zapisz sie do grupy<br/>
[7]Wypisz sie z grupy<br/>
[8]Wyswietl liste uzytkownikow nalezacych do grupy<br/>
[9]Wyloguj i zamknij<br/>

Po pomyślnym zalogowaniu i zakończeniu każdej innej czynności wyświetlane jest menu z dostępnymi operacjami.
Aby wykonać jedną z nich, należy w konsoli podać jej numer. W przypadku 6, 7 i 8
klient zostanie dodatkowo poproszony o podanie nazwy grupy.

### Rodzaje komunikatów

struct wiadomosc {
	long id;
	char username[20];
	char text[1000];
};

<table>
	<tr><td></td><td>Odbierane przez server</td><td>Odbierane przez klienta</td></tr>
<tr><td>id</td><td>ID nadawcy</td><td>ID odbiorcy</td></tr>
	<tr><td>username</td><td>Nazwa adresata (użytkownika /grupy)</td><td>ONazwa nadawcy</td></tr>
	<tr><td>text</td><td>Treść wiadomości</td><td>treść wiadomości</td></tr>
</table>


Używany, kiedy klient chce skontaktować się z innymi użytkownikami.
Zanim zapisze treść wiadomości, musi podać nazwę osoby lub grupy,
z którą zamierza się skontaktować. Klient otrzymuje od serwera zwrotny
komunikat mówiący, czy udało się tę wiadomość wysłać - czyli czy nazwa była poprawna.

-------------------------------------------------------------

struct komunikat {
	long type;
	int success_id;
	long user_id;
	char text[500];
};

<table>
	<tr><td></td><td>Odbierane przez serwer</td><td>Odbierane przez klienta</td></tr>
	<tr><td>type</td><td>Typ komunikatu</td><td>ID odbiorcy</td></tr>
	<tr><td>success_id</td><td>Nieważne</td><td>1 - sukces, 2 - niepowodzenie</td></tr>
<tr><td>success_id</td><td>ID nadawcy</td><td>ID klienta (przy logowaniu_</td></tr>
	
	<tr><td>text</td><td>Nazwa grupy (kiedy jest potrzebna(</td><td>treść komunikatu</td></tr>
</table>

		Odbierane przez serwer			Odbierane przez klienta
type		Typ komunikatu				ID odbiorcy
success_id	Nieważne				1 - sukces, 2 - niepowodzenie
user_id		ID nadawcy				ID klienta (przy logowaniu)
text		Nazwa grupy (kiedy jest potrzebna)	Treść komunikatu

Wykorzystywany przez klientów do zakomunikowania serwerowi chęci
podjęcia akcji innej niż wysłanie lub odebranie wiadomości
oraz przez serwer w reakcji na każdy komunikat.
Wartość numer wskazuje, czy żądanie udało się spełnić, a szczegóły opisuje tekst.

### Typy żądań (komunikatów wysyłamych przez klienta do serwera)

Typ	Żądanie						Uwagi
101	Logowanie	
102	Wylogowywanie	
103	Podgląd aktywnych użytkowników	
104	Podgląd użytkowników należących do grupy	Wymaga podania nazwy grupy
105	Zapisanie do grupy				Wymaga podania nazwy grupy
106	Wypisanie z grupy				Wymaga podania nazwy grupy
107	Wyświetlenie listy grup	


.
