//=====================
//HTML code for webpage
//=====================
const char webpageContManual[] PROGMEM = 
R"=====(
<!DOCTYPE HTML>
<html lang="pl">

  <head>
  <meta http-equiv="refresh" content="600">
     <meta charset="utf-8">
     <title>tensometry.pl</title>
     <meta name="description" content="System pomiarów tensometrami">
     <meta name="keywords" content="tensometry, pomiar masy, mikor">
     <meta name="author" content="Marcin Przyborowski">

     <meta http-equiv="X-Ua-Compatible" content="IE=edge,chrome=1">
  </head>
    <title>Mikor Tensometry</title>
    <!---------------------------CSS-------------------------->
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 20px;
      line-height: 1.6;
    }

    h1, h2 {
      color: #333;
    }

    p {
      margin-bottom: 15px;
    }

    ul {
      list-style-type: none;
      padding: 0;
    }

    li {
      margin-bottom: 8px;
    }

    a {
      color: #007bff;
      text-decoration: none;
    }

    a:hover {
      text-decoration: underline;
    }

    .contact-info {
      margin-top: 20px;
    }

    .portal-address {
      font-weight: bold;
    }
  </style>
</head>

<body>

  <h1>Instrukcja obsługi - System Wciągarki Mikrofonowej Mikor Inżyniering</h1>

  <p>To system Wciągarki Mikrofonowej wyprodukowany przez Mikor Inżyniering. W jego skład wchodzi odbiornik oraz wciągarka mikrofonowa</p>

  <p>Aby połączyć się z urządzeniem, odnajdź sieć TensometryMikor i następnie paruj z użyciem hasła: Mikor1234, następnie uruchom przeglądarkę i wpisz w pasek adresu 192.168.0.1</p>

  <p>W razie problemów skontaktuj się z operatorem w:</p>

  <div class="contact-info">
    <p>Mikor Inżyniering Sp. z o.o.<br>
      Aleja Krakowska 34A, 05-090 Janki<br>
      tel: 728 603 516<br>
      email: <a href="mailto:biuro@mikor-inzyniering.com">biuro@mikor-inzyniering.com</a></p>
  </div>

  <p>Adres portalu to: <span class="portal-address">192.168.0.1</span>.</p>

  <p>Po utworzeniu punktu dostępu powstanie adres <a href="192.168.0.1" target="_blank">192.168.0.1</a>. Użytkownik po połączeniu się z siecią lokalną może adres internetowy i zostanie przekierowany do portalu tensometrów.</p>

  <h2>Kalibracja</h2>

  <p>Urządzenie może być skalibrowane tylko wtedy, gdy jest podłączone do zasilania, tj. ładowarki USB lub komputera.</p>
  
  <p>Serwis internetowy posiada opcję wprowadzenia kalibracji. Przejdź do ustawień wybranego tensometru w serwisie internetowym. Wprowadź współczynnik kalibracji wybranego tensometru.</p>
  
  <p>Nie zaleca się wykonywania kalibracji nadajników tensometrycznych. Aby tego dokonać należy podłączyć się komunikatorem portu szeregowego do ukrytego wewnątrz opakowania portu usb. Po wysłaniu znaku "?" wyświetlą się instrukcje kalibracji.</p>

  <h3>Tarowanie</h3>

  <p>W serwisie internetowy możemy tarować urządzenie. Aby tego dokonać dostań się na serwis internetowy. Kliknij w ustawienia danego tensometru. Dokonaj tarowania poprzez przycisk "Tarowanie Sofwarowe" lub zawierający napis "Tarowanie"</p>

  <p>Urządzenie, nie posiada kompensacji temperaturowej. Odczyty mogą zmieniać się wraz z temperaturą.</p>
  <p>Nadajniki posiadają belkę tensometryczną. Belka tensometryczna ulega odkształceniom sprężystym, a odczyt wymaga stabilizacji w czasie. Wraz z stabilizacją odczyt obciążenia może ulegać zmianom. Wynika to z własności fizycznych stali.</p>

  <p>Programista/dostawca/producent nie odpowiada za operacje kalibracji i tarowania wykonane przez użytkownika</p>

  <h3>Przechowywanie i zimowanie</h3>

  <p>W okresie zimowym i pracy w niskich temperaturach zaleca się częste ładowanie i utrzymanie poziomu baterii powyżej 50%. Po wykryciu utraty kontaktu należy naładować urządzenie w clu zapobegania uszkodzeniom akumulatorów.</p>

  <p>Niezbędnie konieczne i wymagane jest ładowanie urządzenia do maksymalnego poziomu baterii conajmniej w interwałach krótszych niż 70 dni.</p>
  
<h1>Instrukcje</h1>

    <ul>
        <li>Po zakończeniu pracy zalecane jest wyłączenie nadajników. Wyłączenie urządzeń sygnalizowane jest brakiem nadawania pomiarów na serwer tensometrów.</li>
        <li>Nie doporwadzaj do krytycznego rozładowania baterii tensometrów. Grozi to uszkodzeniem akumulatorów. Po wykryciu niskiego poziomu baterii, naładuj je koniecznie. Nienaładowanie grozi uszkodzeniem akumulatora.</li>
        <li>Akumulatory należy utrzymywać w temperaturze dodatniej, czyli powyżej 0 stopni C.</li>
        <li>Unikać zalewania, topienia, utrzymywania w zbyt dużej wilgotności urządzeń.</li>
        <li>Nie rzucać. Nie wyginać. Nie przygniatać urządzeń wchodzących w skład systemu.</li>
        <li>Pamiętaj, że kalibracji samodzielnej dokonujesz na własne ryzyko. Nie wiesz jak to zrobić, to tego nie rób.</li>
        <li>Tarowanie na serwerze powoduje przesunięcie poziome krzywej tensometru. Aby zobaczyć wartość przesunięcia krzywej wyłącz nadajnik tensometru, aby nie wysyłał pomiaru, a następnie zresetuj serwer tensometru. Po ponownym wejściu do serwisu internetowego wyświetlana będzie wartość przesunięcia krzywej tensometru</li>
        <li>Jeżeli nadajnik nie nadaje pomiaru, to może być rozładowany. Jeżeli mimo ładowania nie wysyła pomiaru, podmień pakiet akumulatorowy 1S2P+BMS</li>
        <li>Nie działą i nierozładowany, podłącz kabel USB do portu usb kontrolera ukrytego wewnątrz tensometru. Powinien zacząć nadawać pomiar na porcie szeregowym.</li>
        <li>Użądzenia są naprawialne. Wyprodukowaliśmy je w Polsce, serwisujemy je w Polsce.</li>
        <!-- Dodaj więcej problemów, jeśli to konieczne -->
    </ul>

<a href="/ustawienia_sieci" style="font-size: 16px; color: #007bff; text-decoration: none;">Przejdź do ustawień sieci</a>
</body>

</html>
    </html> 
   )=====";