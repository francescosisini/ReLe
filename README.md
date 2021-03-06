# ReLe
Libreria didattica per la creazione, addestramento e test di reti neurali fino a tre strati in linguaggio C

Il progetto ReLe punta a realizzare una libreria con diversi algoritmi utili nell'implementazione di logiche AI in progetti di sviluppo reale
La strategia è di mantenere un'alta leggibilità del codice che insieme a modelli documentati permetta il suo utilizzo modificando il codice in base alle necessità anziché prevedendo tutte le possibili declinazioni in anticipo, in questo modo la dimensione e la leggibilità dovrebbero preservarsi
#### Sponsor 
[Introduzione alle reti neurali: con esempi in linguaggio C](https://www.amazon.it/Introduzione-alle-neurali-esempi-linguaggio/dp/1692945319)

## Caratteristiche di ReLe
- **Re**te neura**Le** mulistrato (completamente connessa) in linguaggio C
- Addestramento con Backpropagation
- Addestramento con Algoritmi Genetici (per ora solo per reti ad 1 strato)
- Facile da usare: bastano 3 chiamate per creare, addestare e provare una rete
- Possibilità di salvare la rete in formato CSV (Tabulazioni al posto delle virgole TSV)
- Caricare una rete da file con una sola chiamata
- Analizzare la struttura delle connessioni con Calc (o Excel) o con un editor di testo
- Modificare i pesi delle connessioni manualmente per vedere gli effetti
- Valutare l'errore quadratico della rete per sapere quando completare l'addestramento

- Multipiattaforma

## Installazione
Clonare il repository

`git clone https://github.com/francescosisini/ReLe.git`

Entrare nella directory  `ReLe` dalla directory di clone: `cd ReLe`

Lanciare `make`

## Esempi d'uso
Per testare la libreria e provare ad usarla nelle proprie applicazioni guardare gli esempi della cartella [esempi](esempi)
## Modello
Il codice di `ReLe` è coerente al modello presentato nella figura ![Modello ReLe](img/MLP_detailed_3S.png)


