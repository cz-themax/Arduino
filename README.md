#Arduino -  Dálkové ovládání pomocí SMS
Prikazy:
gloz - otevre nebo zavre levou garaz
gpoz - otevre nebo zavre pravou garaz
broz - otevre nebo zavre branu
pepa - otevre pravou garaz a pokud je zapnute potvrzovani, odesle info na +420777132363
ping - pro zjisteni, zda-li modul zije, modul posle zpet SMS
potvrz - zapne potvrzovani prikazu a ulozi do EEPROM - pri kazdem prikazu na otevreni nebo zavreni odesle SMS s informaci o vykonanem prikazu
nepotvrz - vypne potvrzovani prikazu a ulozi do EEPROM v pripade ze je zapnute potvrzovani a prijde SMS s neznamym prikazem, modul oznami SMSkou
neznamy prikaz
