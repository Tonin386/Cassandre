# SY22 Notice

Ce répositoire permet de créer une structure de communication de partage d'informations en utilisant un module XBee et des ESP32 (voir le schéma). Nous présentons ici la partie concernant le terrain de Basket : comment le mettre en place et l'utiliser.

![Schéma du projet](https://i.ibb.co/47NRstg/Sch-ma-projet.png)

## Matériel nécessaire

 - 2 modules ESP32 WROOM 32  
 - 1 module ESP32 WROVER IE 
 - 1 XBee Grove Dev Board + 1 module XBee S2C
 - (Optionnel) 1 capteur Grove 6-axis Accelerometer & Gyroscope

Il faut aussi au préalable installer XCTU ainsi que Arduino IDE. 
**Vérifiez de bien posséder tous les drivers nécessaires pour brancher vos modules sur les ports USB, sinon vous ne pourrez pas voir les modules sur votre ordinateur !** 

### Téléchargement XCTU
Windows (64 bits) : https://hub.digi.com/support/products/xctu/?path=/support/asset/xctu-v-659-windows-x86x64/
Mac : https://hub.digi.com/support/products/xctu/?path=/support/asset/xctu-v-658-macos-x/
Linux (64 bits) : https://hub.digi.com/support/products/xctu/?path=/support/asset/xctu-v-659-linux-x64/

### Téléchargement Arduino IDE

Lien avec les versions Windows, Mac et Linux : https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE

### Téléchargement des Drivers

Voici la liste des drivers à installer. Veillez à bien choisir la version appropriée à votre OS :

 - Driver CP210x : https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads
 - Driver CDM : https://ftdichip.com/drivers/d2xx-drivers/
 - Driver VCP : https://ftdichip.com/drivers/vcp-drivers/
 - Driver Digi XBee S2C : https://hub.digi.com/support/products/digi-xbee/digi-xbee-s2c/?path=/support/asset/xbee-zb-s2c-firmware-4061/


## Mise en place des équipements

### Configuration du module XBee - XCTU

Ouvrir XCTU et brancher le XBee Grove Dev Board à votre ordinateur.
Pour afficher votre module XBee sur XCTU, cliquer sur 
**Discover radio modules**
*Choisissez le port USB sur lequel votre XBee est branché : **USB Serial Port**, si aucun port n'apparaît, vérifiez l'installation de vos drivers.*
**Next**
*Ne changez rien*
**Finish**
*Il scanne...*
*Sélectionnez votre module*
**Add selected devices**

Dans la liste des modules, sélectionnez celui qui apparaît désormais. Dans la zone de configuration, cliquez sur le bouton 
**Update**
*Sélectionnez 802.15.4 TH > 2003 (Newest)*
**Update**

Ensuite éditez ces champs comme suit : 

 - Channel : `C`  *Définit le canal de communication avec les autres modules* 
 - PAN ID : `2244` *Définit l'adresse du canal* 
 - Destination Address Low : `2235` *Définit l'adresse du module routeur qui reçoit toutes les données* 
 - 16-Bit Source Address : `300` *Optionnel, l'adresse de votre module*)

**Write**

Votre XBee est bien configuré ! 

### Branchements
Une fois les logiciels et les drivers installés, on peut procéder à la mise en place des équipements. Il faut brancher la XBee Grove Dev Board (sur laquelle est inséré le module XBee S2C) à un ESP32 WROOM 32 en utilisant ce branchement : 

![Branchement XBEE - ESP32](https://i.ibb.co/56H2rN8/Branchement-XBEE-ESP32-1-1.png)

 (Optionnel) Branchement du capteur sur l'autre carte ESP32 WROOM 32 :
![Branchement Capteur - ESP32](https://i.ibb.co/kKng51s/Branchement-Capteur-ESP32.png)

### Installation des cartes et des bibliothèques dans Arduino IDE
 Tout d'abord, il faut ajouter les cartes ESP32 à l'IDE, car elles ne sont pas disponibles de base. 
 **File -> Settings** 
 *Une fenêtre apparaît* 
 **Additional board manager URLs** :
 *Ajouter cette URL :* `https://dl.espressif.com/dl/package_esp32_index.json`
 
 On peut désormais installer les cartes ESP32. 
 **Tools -> Board: -> Board Manager...**
 *Dans la zone de texte, rechercher **ESP32*.**
 **Install**
 
Les cartes ESP32 sont installées ! Maintenant, les bibliothèques.

Pour installer une bibliothèque dans l'IDE : 
**Tools -> Manage Libraries...**
*Dans la zone de texte, rechercher la bibliothèque voulue, puis cliquer sur*
**Install**
 
 Voici la liste des bibliothèques à installer : 
 1. ESPAsyncTCP *by dvarrel*
 2. ESPAsyncWebSrv *by dvarrel*
 3. AsyncHTTPRequest_Generic  *by Bob Lemaire, Khoi Hoang...*
 4. ESPAsync_WiFiManager *by Khoi Hoang*
 5. EspSoftwareSerial *by Dirk Kaar, Peter Lerup*
 6. Seeed Arduino LSM6DS3 *by Seeed Studio*

Maintenant, vous pouvez téléversez les codes sur les cartes comme préciser ci-dessous.

    http_client > ESP32 câblé au module XBee
    http_server > ESP32 câblé au capteur
    http_relay > ESP32 seul

Vous pouvez ouvrir les consoles Serial afin d'obtenir des informations sur le déroulé des programmes.
