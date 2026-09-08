# Progetto di Computer Grafica: Plants vs Zombies

**Autore:** Alessandro Bozzini 6400176

**Corso:** [Fondamenti di Computer Grafica]

SFML 3 (Simple and Fast Multimedia Library)

---

## Introduzione

Il progetto consiste nello sviluppo di un videogioco ispirato a Plants vs Zombies, realizzato in C++ con l'utilizzo della libreria SFML 3. 
L'obiettivo è implementare una versione semplificata del gioco, gestendo la logica di gioco, la grafica a sprite, le animazioni e l'interazione con l'utente tramite input da mouse. Il progetto è sviluppato incrementalmente attraverso una serie di 10 Tappe, ognuna delle quali aggiunge nuove funzionalità e complessità al gioco.

---

## Obiettivo del gioco
L'obiettivo del gioco è di raccogliere i soli (sia che cadono dal cielo che dai girasoli) per piantare piante (girasoli, 'peashooter' o 'wallnut') per difendere la propria casa dagli zombie, che entrano dal lato destro dello schermo e si muovono lungo le 5 righe verso sinistra.

Quando uno zombie incontra una pianta, inizia a mangiarla provando a distruggerla. I peashooter possono sparare colpi per uccidere gli zombie

Ho deciso di implementare due livelli, dove nel secondo sono presenti zombie più forti

---

## Struttura del Progetto
Per le prime 4 tappe è presente solo un file main.cpp, dopodichè il progetto è organizzato in diversi file sorgente e header:

| File | Descrizione |
| :--- | :--- |
| `main.cpp` | Contiene il loop principale del gioco, la gestione degli eventi e la logica di gioco principale. |
| `pvz.h` | Header che contiene le classi principali (`Plant`, `Zombie`, `Sun`, `Pea`), le costanti e le dichiarazioni delle funzioni. |
| `pvz.cpp` | Implementazione delle funzioni di gestione delle texture, del disegno della griglia e del caricamento delle risorse. |
| `animation.h` | Classe `Animation` per gestire il rendering degli sprite sheet (animazioni a frame). |

Le risorse grafiche (immagini, font) sono contenute nella cartella `resources/`.

---

## Descrizione delle Tappe

### Tappa 01: Inizializzazione e Finestra
- **Obiettivo:** Aprire una finestra SFML e disegnare lo sfondo.
- **Funzionalità implementate:**
    - Creazione della finestra.
    - Caricamento e rendering dell'immagine di sfondo.
    - Impostazione del limite di framerate.
- In questa tappa, l'importante era costruire una base che compilasse e funzionasse, su cui iniziare a lavorare

### Tappa 02: Griglia e piante
- **Obiettivo:** Creare una griglia e piazzare le piante
- **Funzionalità implementate:**
    - Caricamento delle texture delle piante.
    - Piazzare le piante con offset corretti nella griglia
- Per aiutarmi ho creato una funzione che traduce l'indice della griglia (array 2d) in coordinate

### Tappa 03: Zombie
- **Obiettivo:** Spawning degli zombie e movimento
- **Funzionalità implementate:**
    - Caricamento texture zombie
    - Movimento degli zombie verso la casa
- Qua gli zombie sono ancora png statici, più avanti ho implementato l'animazione

### Tappa 04: Piazzamento delle piante
- **Obiettivo:** Permettere all'utente di piantare le piante
- **Funzionalità implementate:**
    - Creare le icone con le texture giuste.
    - Implementare una selezione di quale pianta si vuole piazzare cliccando sulle icone
    - Piazzare le piante sulla griglia in base a dove clicca l'utente

- Ho dovuto convertire la posizione di dove clicca l'utente in indici della griglia

### Tappa 05: Refactoring
- **Obiettivo:** Rendere più pulito e leggibile il codice
- **Funzionalità implementate:**
    - aggiunta dei file pvz.h e pvz.cpp, con rispettive classi Zombie, Pea, Sun... (anche se alcune non ancora utilizzate)
- Questa tappa non aggiunge niente al gioco effettivo, ma mi permette di avere del codic su cui è più facile lavorare ed aggiungere nuove funzionalità

### Tappa 06: Spawning dei soli e piazzamento delle piante col costo
- **Obiettivo:** Permettere all'utente di raccogliere i soli e usarli per piazzare le piante
- **Funzionalità implementate:**
    - Spawning dei soli con clock
    - Raccoglimento dei soli tramite click con il mouse
    - Possibilità di spendere i soli per piazzare piante

### Tappa 07: Logica di combattimento e spawning degli zombie
- **Obiettivo:** Far spawnare randomicamente gli zombie e far sparare alle piante contro gli zombie
- **Funzionalità implementate:**
    - I peashooter ora sparano quando vedono uno zombie nella loro riga
    - Gestione delle collisioni dei colpi con gli zombie
    - Gli zombie possono morire
    - Gli zombie spawnano randomicamente con un clock

### Tappa 08: More refactoring and animation
- **Obiettivo:** Risolvere diversi problemi con il codice e aggiunta animazione della camminata
- **Funzionalità implementate:**
    - Aggiunta la classe Animation.h, per gestire le animazioni
    - Aggiunta l'animazione della camminata degli zombie
    - Funzioni molto grandi divise in spawner e handler (che rispettivamente creano e gestiscono gli sprite di soli, pea e zombie)
    - Utilizzo di un singolo clock
    - Cambiamento di costanti come danno, vita degli zombie
- Queste costanti vengono cambiate spesso nelle tappe, nella tappa10, a gioco finito, sono appropriate per una partita simile al gioco ufficiale

### Tappa 09: Animazione dello zombie che mangia le piante + nuova pianta 
- **Obiettivo:** Aggiungere nuova animazione, nuova pianta, e gestire meglio il passaggio di livello
- **Funzionalità implementate:**
    - Nuovo zombie (con più vita)
    - Nuova animazione, per entrambi i tipi di zombie
    - Nuova pianta ('wallnut') che funge da muro (con 1000 punti vita)
- La parte più complicata è stata adattare gli sprite sheet

### Tappa 10: Plants vs Zombies
- **Obiettivo:** Finalizzare il gioco.
- **Funzionalità implementate:**
    - Aggiunto passaggio di livello funzionante
    - Aggiunto contatore degli zombie mancanti
    - Schermata di vittoria e chiusura del programma

---

## Commenti
Le difficoltà più grandi sono state riscontrate creando e aggiustando gli sprite sheet per le animazioni (infatti l'animazione di quando gli zombie mangiano non è molto precisa). Per farlo ho utilizzato GIMP (GNU Image Manipulation Program), un software open source per la manipolazione di immagini, lavorando su sprite e immagini trovate online, principalmente sulla wiki del gioco ufficiale, che dispone di una galleria con svariate immagini (sia ufficiali che fan-made). Per quanto le risorse fossero limitate, sono riuscito a creare un gioco fedele a quello ufficiale, che era il mio obbiettivo. 

Se si volesse estendere il codice e continuare il progetto occorrerebbe effettuare ulteriore refactoring, in quanto il codice contiene svariate costanti "hard-coded" che non è molto elegante, ed effettua un sacco di funzionalità nel file main.cpp, quando sarebbe meglio dividere il codice in più file.