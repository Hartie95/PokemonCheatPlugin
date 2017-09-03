# ポケットモンスター マルチチートプラグイン

これは `NTR CFW`用のチートプラグインで、以下の世代に対応しています。

* ポケットモンスター X
* ポケットモンスター Y
* ポケットモンスター オメガルビー
* ポケットモンスター アルファサファイア

フォーク元: https://github.com/hartmannaf/PokemonCheatPlugin

このプロジェクトの目標は以下の通りです。

* 日本語主導での開発
* 日本語ドキュメントの追加
* ビルドの簡素化 (`Docker` 等を使って)
* 新しいチートの追加
* (オプション) その他の世代のチートの追加

## チート一覧
野生ポケモンの変更を適用するためには、
今いるエリアとは別のエリアに移動しなければならないようです。

### 野生ポケモンの生成ルール変更 (作者: [MrNbaYoh](https://github.com/MrNbaYoh))

* 全てのポケモンを色違いにする

### 野生ポケモンの変更
* ポケモンの種類を ランダムに / 1番目の道具個数で 変更
* 常に色違い (作者: [MrNbaYoh](https://github.com/MrNbaYoh))

### 野生ポケモンのバリエーション変更
* ランダムに / 2番目の道具個数で 変更

フォルムやメガシンカ等の状態を指定します。例えば、1番目の道具の個数を
382個にして、2番目の道具の個数を 2個にすると、ゲンシカイオーガになります。
2番目の道具の個数を 1個にすると、普通のカイオーガになります。

### 野生ポケモンのレベルを変更
* ランダムに / 3番目の道具個数で 変更
* ずかんナビも出現するポケモンに合わせて更新 (ORAS のみ)


### 道具変更
* **道具の種類と個数を変更**

* 1番目の道具を 999個にする
* 2番目の道具を 999個にする
* 3番目の道具を 999個にする

### バトル中 (作者: [MrNbaYoh](https://github.com/MrNbaYoh))
* HP 無限
* PP 無限
* 捕獲率 100%
* 経験値 1,000,000
* 敵ポケモンの残り HP 1

### その他
* タマゴの孵化を早める
* Oパワー無制限
* 常にむしよけスプレー

## ベータ版
* 全ての道具を入手 (999個)
* お金 Max

-------------------------------------------------------------------

### Usage

#### The plugin

First ntr cfw needst to be setted up on your 3ds, and for full support you need to install the latest update for your pokemon edition.

After this is done, simply copy the plugin in one of the following folders(depending on your game):

* X:  /plugin/0004000000055D00/
* Y:  /plugin/0004000000055E00/
* OR: /plugin/000400000011C400/
* AS: /plugin/000400000011C500/

Keep in mind, that only one ntr plugin is supported at the same time.

#### Cheats

* Wild encounter modifier(Item bag)
   * Species
       * Press L+A to set the first item count to 721
       * Modifie the item count using:L+UP(+1)/L+DOWN(-1)L+RIGTH(+10)/L+LEFT(-10) 
 * Variation
       * Press L+A to set the second item count to 27
       * Modifie the item count using:L+B+UP(+1)/L+B+DOWN(-1)L+B+RIGTH(+10)/L+B+LEFT(-10) 
 * Level
       * Press L+A to set the third item count to 100
       * Modifie the item count using:L+X+UP(+1)/L+X+DOWN(-1)L+X+RIGTH(+10)/L+X+LEFT(-10) 
* ItemModifier
    * Item type(uses the medicine bag items)
        * Press R+A to set the first three medicine counts item count to 1
        * Modifie the item id(third medicine) using:R+UP(+1)/R+DOWN(-1)R+RIGTH(+10)/R+LEFT(-10)
        * Modifie the item bag(second medicine) using:R+B+UP(+1)/R+B+DOWN(-1)R+B+RIGTH(+10)/R+B+LEFT(-10)  
        * Modifie the item position(first medicine):R+X+UP(+1)/R+X+DOWN(-1)R+X+RIGTH(+10)/R+X+LEFT(-10)


#### Supported itemBags

1. Item bag
2. Medicine bag
3. TM/HM bag
4. Berry bag
5. Key item bag


### Additional informations:

#### Item ID list

The Items IDs Are listet id the wiki, please help to fill it:
[Item ID Wiki](https://github.com/hartmannaf/PokemonCheatPlugin/wiki/itemList)

#### Shiny Encounter
[How the shiny encounter works](https://github.com/hartmannaf/PokemonCheatPlugin/wiki/Shiny-PID-Calculation)

### Credits
* Encounter modifier base plugin [Pokemon Randomize](https://gbatemp.net/threads/pokemon-randomize-a-pokemon-x-y-or-as-ntr-cfw-plugin.397096/) by [KazoWAR](https://gbatemp.net/members/kazowar.133086/)
* Multiple cheats and fixes where done by [MrNbaYoh](https://github.com/MrNbaYoh)
