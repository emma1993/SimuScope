# VRPN-Plugin für die Unreal Engine

VRPN (http://www.cs.unc.edu/Research/vrpn/) ist ein geräteunabhängiges, netzwerktransparentes System für die Kommunikation mit und zwischen Geräten und Anwendungen der virtuellen Realität. Es erlaubt über Interfaces auf mehrere Geräte gleichzeitig zuzugreifen, verwaltet dabei die Verbindung und führt eine zeitliche Synchronisation aus.

Obwohl viele Geräte mit VRPN kompatibel sind, existiert bislang keine Integration mit der Unreal Engine. Aus diesem Grund wurde im Sommersemester 2015 an der Hochschule Reutlingen ein Plugin für die Unreal Engine entwickelt, das VRPN-Trackerdaten (vrpn_Tracker) per C++ oder per Blueprint in der Engine zur Verfügung stellt.

Im vorliegenden Anwendungsfall wird dieses Plugin genutzt, um Daten eines WorldViz-Trackingsystems (http://www.worldviz.com/) bereitzustellen.

## Voraussetzung

* 64-bit System
* Unreal Engine Version 4.10
* Visual Studio 2015 (für VS2013 s.u.)

## Plugin-Installation

Plugins werden grundsätzlich für jedes Unreal Projekt durch das Kopieren des Plugin-Ordners ("VrpnPlugin", der Ordner der diese Datei enthält) in den "Plugins"-Unterorder im Projektverzeichnis installiert. Sollte dieser Ordner nicht vorhanden sein, muss er erstellt werden.

Das Laden eines Plugins in der Unreal Engine setzt aktuell C++-Quellcode voraus. Besteht das Projekt lediglich aus Blueprints und Unreal Content, muss eine leere C++-Klasse zum Projekt hinzugefügt werden. Dies ist in der Engine über den Menüeintrag "File | Add Code to Project..." möglich. Nach eine Nuestart der Engine kann über das Plugin-Menü geprüft werden, ob dieses gefunden und aktiviert ist.

Um die Daten in einem Blueprint empfangen zu können, wird zunächst das Blueprint des Charakters geöffnet. Auf der linken Seite des Fensters findet sich die Komponentenliste. Durch "Add Component" kann nun die "Vrpn"-Komponente hinzugefügt werden. Ist diese hinzugefügt, muss für das Empfangen von Events außerdem das Interface zum Blueprint hinzugefügt werden. Dafür werden die "Class Settings" (ehemals "Blueprint Props") aufgerufen und das "Vrpn Inteface" über die Schaltfläche "Add" im Abschnitt "Interfaces" hinzugefügt. Danach muss das Blueprint kompiliert werden. Im Anschluss daran können Events wie jeder andere Blueprint-Knoten hinzugefügt und eingesetzt werden. Der VRPN Server von dem die Daten empfangen werden sollen kann in den Eigenschaften der "Vrpn"-Komponente eingetragen werden. Selbes gilt für den Trackernamen.

## Nutzen der Daten

Die Daten können über das Event "On Vrpn Tracker Data" empfangen werden. Dieses Event wird für jeden Tracker einzeln aufgerufen, wobei durch den ersten Ausgang dessen ID angegegben wird. Die weiteren Ausgänge für die Position und die Orientierung sind je nach Verfügbarkeit gefüllt und in den Standarddatentypen FVector und FRotator angegeben. Im Bild vrpnEvent-example-blueprint ist ein Beispiel abgebildet.

## Use of tracker data

Tracker data is received by the event "On Vrpn Tracker Data". This event gets called for each tracker id individually. By using the id you can differentiate the data of position (FVector) and orientation (FRotator) for each tracker. See example in vrpnEvent-example-blueprint image.

## Einsatz von Visual Studio 2013 (nicht getestet da nicht mehr offiziell durch Unreal unterstützt)

Für den Einsatz von Visual Studio 2013 muss die externen VRPN Bibliothek ebenfalls durch VS2013 erstellt worden sein.
Das Archiv enthält diese Version ebenfalls. Für den Einsatz muss jedoch manuell zu dieser Version gewechselt werden:

In der Datei VrpnPlugin.Build.cs im Quellcode-Verzeichnis des Plugins muss in Zeile 85 "VS2015" mit "VS2013" ersetzt werden.

Daraufhin sollte das Plugin und das komplette Unreal-Projekt ohne Fehler neu erstellt werden können.

## License

This software may be modified and distributed under the terms
of the MIT license. See the LICENSE file for details.

The Software makes use of third party software. Use of some third party materials included in the Software are subject to the third party terms and conditions in the LICENSE-3RD-PARTY file, in addition to the terms and conditions included above.

