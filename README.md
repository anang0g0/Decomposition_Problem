# Key Exchange by Discrete Logarithm Problem of Near-Field

# 20230708

どうやら有限体を使っているので半分配体であるらしい。

# 20230704

Near-Ringはこのことだったの？という感じで、現在半分配環を勉強中。  
半直積に楕円曲線を使って、それに加法群を導入したら左分配法則しか満たさない半分配環になったというお話。
それでもきちんと動いているのは分配則を使ってないから。

結合則だけを使う分には問題なし。

# 20230630
まだ途中です。

https://qiita.com/fumumue/items/a5451eb51ae6464dfe37

https://qiita.com/fumumue/items/5f622ee6ea83cbd4a7f9

https://qiita.com/fumumue/items/55b61f63a005f290a2c6

# 目的
今回はこの３つの過去記事からできた新型暗号の攻撃法について調べます。
考えられるのは、複数の原始根からできた複数の離散対数をガウスの消去法を使って解読することが目標です。

# 準備：
半直積の基本演算を定義すると、

$(c,s)(d,t)=(cd,sd+t)$：積
$(c,s)^{-1}=(c^{-1},-sc^{-1})$：逆元
$(c,s)^{-1}(d,t)(c,s)=(d,s(e-d)+tc)$：共役元
$(c,s)^n=(c^n,c^{n-1}s+c^{n-2}s+...+cs+s)$：べき乗公式

ここで$n$項等比数列の総和が$S_n=\frac{(c^n-1)}{c-1}$から、

$(c,s)^n=(c^n,\frac{c^{n-1}-1}{c-1}s)$
$(c,s)^{nm}=(c^n,\frac{c^{n-1}-1}{c-1}s)^{m}=(c^{n},s')^m=
(c^{nm},\frac{c^{m-1}-1}{c-1}s')$
$(c,s)^{m+n}=(c^{m+n},c^m\frac{c^{n-1}-1}{c-1}s+\frac{c^{m-1}-1}{c-1}s)$

**定義：原始根**
「$p$は素数であるとする。巡回群$Z^\*_p$の生成元$g$は$Z^*_p$の原始根(primitive root)またはmodulo p　の原始根と呼ばれる。」

秘密鍵：$s,t,u,x,y,z \in Z_p,B \in E,s > u, x > z$
公開鍵：$A,C,D,E \in E,D=A^sB^tC^u,E=A^xB^yC^z$
$p,q,r \in Z_p$ の異なる原始根。
$P,Q,R \in E$　は同じ楕円曲線上の異なる位数の点。
つまり、$A=(p,P),B=(q,Q),C=(r,R)$である。

・ある数$g$が原始根かどうかを確かめる方法
定義体がソフィー・ジェルマン素数であれば、$P-1=2*Q$($Q$は素数)と表せるので、次の条件を満たすかどうかで原始根かどうか判定することができる。

判定法：ある整数$x$が原始根であるとき、
$x^{(P-1)/2} \bmod P \neq 1$
かつ、
$x^{2} \bmod P \neq 1$
これが安全素数に対する原始根の判定条件になります。

例えば、定義体
$P=115792089237316195423570985008687919879869678752368678225506334468897341715723$
$p=2,q=4,r=6$の場合を考えると、
$4$は原始根ではなく、$2,6$は原始根である。

# 暗号１：MELIKOの改良版
ケーリーハミルトン攻撃を考慮した行列暗号のつもり。←これを半直積に置き換える。

秘密鍵:
整数 $a,b,c,d$
ただし、 $a \neq b$
素体の元と素体上で定義された楕円曲線上の点との半直積$A$
公開鍵:
上と同じ半直積の元、
$X$,
$Y = X^a A^c X^{-a}$,
$Z = X^b A^{cd} X^{-b}$,
暗号化:
M: 平文、 r,s: 乱数
$C1 = X^r Y^s X^{-r}$
$C2 = X^r Z^s X^{-r}$
暗号文(C1, C2 * M)

復号:
$C1 = X^{(r+a)} A^{sc} X^{-(r+a)}$
$C2 = X^{(r+b)} A^{scd} X^{-(r+b)}$
より、
$C2 = X^{(b-a)} C1^d X^{-(b-a)}$
C2*M に、上で求めたC2の逆元を掛けて M を得る。

# MELIKOに対する攻撃
まず、半直積は行列として記述することもできる。
つまり$XY=(a,A)(b,B)=(ab,bA+B)$は、

```math
\begin{pmatrix}
 a & A \\ 
 0 & 1
\end{pmatrix}
\begin{pmatrix}
 b & B \\ 
 0 & 1
\end{pmatrix}
```
こう書くこともできるということだ。なので、
公開鍵 Y,Z の式を以下のように変形し、Aを消去する。
$Z = X^{(b-a)} Y^d X^{-(b-a)}$
ここで$e=b-a$と置くと
$Z=X^eY^dX^{-e}$

「更に、$ZX^e=X^eY^d$と変形し、これをケーリーハミルトンの定理を使って書くと
$Z(tX+uI)=(tX+uI)(vY+wI)$
となる。よって、式を展開すると、
```math
\begin{pmatrix}
 z & Z \\ 
 0 & 1
\end{pmatrix}
\begin{pmatrix}
 ta+u & tA \\ 
 0 & t+u
\end{pmatrix}
=
\begin{pmatrix}
 ta+u & tA \\ 
 0 & t+u
\end{pmatrix}
\begin{pmatrix}
 vb+w & vB \\ 
 0 & v+w
\end{pmatrix}

```
$o=ta+u,p=t+u,q=vb+w,r=v+w$
と置くと、
```math
\begin{pmatrix}
 zo & ztA+pZ \\ 
 0 & p
\end{pmatrix}
=
\begin{pmatrix}
 oq & ovB+trA \\ 
 0 & pr
\end{pmatrix}
```
$X,Y,Z$ は既知なので、連立方程式を解けば $t,u,v,w$が決まる。ゆえに、
$X^e,Y^d$が得られる。」

はずなのだが、式は３つだけで、中には未知係数を楕円曲線の点にかけている部分もあり、解けるとは思えない。この計算の精密な解析は後に詳しく行うことにして、先に進む。ここまでの解析は多項式を使った方法であった。
そこで以下の議論を使うことにする。

• Y と $Y^d$ から d が求まれば、復号に必要なすべての情報が揃う。
◦ これは DLP (Discrete Logarithm Problem) である。
◦ AとXが置換行列の場合、これは解けることがわかっている。
◦ 一般の行列においては困難。
C1 の式をケーリーハミルトン攻撃して、 $Y^s$ を得る。
• $Y^d$ と $Y^s$ から $Y^{ds}$ が求まれば、 $A^{scd}$ を入手できる
◦ $C1' = X^{(b-a)} C1^d X^{-(b-a)}$ の式を攻撃するには $A^{scd}$ が必須。
◦ これは DHP (Diffie–Hellman problem) であり、一般に困難

実際には、すべての要素が有限体の元であるような行列の離散対数問題を解読する方法はすでに知られており、他の材料で作ることが望ましい。かつ、離散対数問題はすでに量子計算機で解読できることが知られていて、もはや安全とはいえない。問題はこの行列がスカラーと楕円曲線の点群の混合であることである。このような行列の離散対数問題がどのくらい難しいかというところが問題である。

なので「普通の離散対数問題は死んだ！」と言いたいところではある。
だが、半直積のべき乗は以下の式で表される。
$(c,S)^n=(c^n,\frac{c^{n-1}-1}{c-1}S)$（Sは楕円曲線の点）
これを行列表現にすると、
```math
\begin{pmatrix}
 c^n & \frac{c^{n-1}}{c-1}S \\ 
 0 & 1
\end{pmatrix}
```
暗号１を解読するためには、この行列から秘密指数$n$を計算する必要がある。

# 暗号２：多元離散対数問題（乗法に関する）
秘密鍵：$s,t,u,x,y,z \in Z_p,B \in E,s > u, x > z$
公開鍵：$A,C,D,E \in E,D=A^sB^tC^u,E=A^xB^yC^z$
$p,q,r \in Z_p$ の異なる原始元。
$P,Q,R \in E$　は同じ楕円曲線上の異なる位数の点。
つまり、半直積に使う元、$A,B,C$は、それぞれ有限体の元$p,q,r$と楕円曲線の異なる点$P,Q,R$とを任意に取り、
$A=(p,P),B=(q,Q),C=(r,R)$
である。


**定義：原始根**
「$p$は素数であるとする。巡回群$Z^\*_p$の生成元$g$は$Z^*_p$の原始根(primitive root)またはmodulo p　の原始根と呼ばれる。」

秘密鍵：$s,t,u,x,y,z \in Z_p,B \in E,s > u, x > z$
公開鍵：$A,C,D,E \in E,D=A^sB^tC^u,E=A^xB^yC^z$
$p,q,r \in Z_p$ の異なる原始根。
$P,Q,R \in E$　は同じ楕円曲線上の異なる位数の点。
つまり、$A=(p,P),B=(q,Q),C=(r,R)$である。

・ある数$g$が原始根かどうかを確かめる方法
定義体がソフィー・ジェルマン素数であれば、$P-1=2*Q$($Q$は素数)と表せるので、次の条件を満たすかどうかで原始根かどうか判定することができる。


$D'=(p,P)^s(q,Q)^t(r,R)^u=(p^sq^tr^u,q^tr^u\frac{p^{s-1}-1}{p-1}P+r^uQ+\frac{r^{u-1}-1}{r-1}R)$
$E'=(p,P)^x(q,Q)^y(r,R)^z=(p^xq^yr^z,q^yr^z\frac{p^{x-1}-1}{p-1}P+r^zQ+\frac{r^{z-1}-1}{r-1}R)$

この半直積の左側成分は通常の多元離散対数問題であり、右側は楕円曲線の多元離散対数問題である。
２つの異なる原始根を使った離散対数問題は、「多元離散対数問題（Multiple Discrete Logarithm Problem）」と呼ばれます。これは、異なる原始根に対する離散対数を求める問題です。

暗号文：
$C1=A^rDC^{r'}=A^{(s+r)}BC^{(u+r')}$
$C2=A^rEC^{r'}=A^{(x+r)}BC^{(z+r')}$
復号
アリスは$s-x,u-z$を知っているので以下を計算できる。
$X=A^{(s-x)}C2C^{(u-z)}=A^{(x+r)}BC^{(y+r')}=C1$

暗号化に関する指数にはなんの制約もないので、むしろこっちの設定のほうが自然だと思います。秘密鍵が増えるということ以外、特に問題はなさそうです。

この暗号は一見して難しそうに見えます。
というのも右要素も左要素も構造が複雑だからです。
2つ以上の原始根、それも2つ以上の秘密指数がある場合には、互いに混ざり合ってしまうので、単純には計算できません。これはもう少し論文を理解する必要があるので簡単には言い切れないですが、解読ができてもできなくても、ここで公開する予定です。
原始根$q$は秘密鍵であり、更に両側の原始根を秘密指数でべき乗しているので簡単には指数を計算できません。ただ、$p,r$は小さなサイズの原始根、$q$は大きなサイズの原始根であるとします。
ただ左半分は、右の楕円曲線を計算するためのものであって、楕円曲線の521bit以上の大きさにはなりません。そして見たことのない3つの原始根の離散対数問題になっています。
これが解ければ楕円半直積の離散指数も分かるということなので、この暗号の安全性は半直積の離散対数問題にある可能性が濃くなります。
素数$p,q,r$がそれぞれ異なる原始根であることから、簡単な指数関数としてみることもできません。
同じような方式がないか調べる必要があります。（それが今日多元離散対数問題であるということが解った）

### 暗号文解析：

$X=A^{(x-z)}C2C^{(y-w)}=A^{(x+r)}BC^{(y+r')}=(p^{x+r'}qr^{y+r'},qr^{y+r'}\frac{p^{x+r'}-1}{p-1}P+r^{y+r'}Q+\frac{r^{y+r'}-1}{r-1}R)$

暗号文は最終的にこの形に収まります。
この暗号の計算には、楕円曲線の点の素数位数$n$を使います。
行列の場合、未知変数が2つだったから解読できただけで4つにしたら安全だったのかもしれません。
暗号化に使う点のスカラー倍は半直積のべき乗で等比級数の総和で決まりますが、これは半直席の特徴です。
この点のスカラー倍は、秘密指数が分からなければ決定できないので、ここはやはり半直積の離散対数に依存していると言えるような気がします。
暗号文から秘密鍵や平文の情報が漏れているようには見えません。
行列じゃないからケーリーハミルトンも使えないし、総合してみると、$A,C$を公開しても全く問題がなさそうなので、早速実装してみました。

$(y,P,g_i)$が公開パラメータとして、素数$P$、互いに素な$i$個の原始元$g_i$、$i$個の秘密指数$x_i$があった時、
$$y=\Sigma^n_{i=0}g_i^{x_i}$$
$(y,P,g_i)$から$(x_0,x_1,...,x_n)$を求める問題を（加法的）多元離散対数問題：(Additive Multi Discrete Logarithm Problem)と呼ぼう。

ベクトル同士の積が定義されてないように、半直積の和も定義次第でどうにかなりそうな気がするが、どうすれば矛盾なく計算できるのか、或いは左側だけ演算を変えることができるのか、しばらく時間が必要である。
もしこれがうまく行けば、難しい問題になる可能性あり。

# 暗号３：加法バージョン
ここで、半直積同士に加法を定義してみる。つまり、
$(p,P)+(q,Q)=(p+q,P+Q)$
$(a,A)^n+(b,B)^m=(a^n+b^m,\frac{a^{n-1}-1}{a-1}A+\frac{b^{m-1}-1}{b-1}B)$
よって、加法に関しては、可換である。
逆元は、$(a,A)^{-1}=(-a,-A)$である。

## 定義：多元離散対数問題（加法と乗法に関する）
1st stepと同じように材料を定義する。
秘密鍵：$s,t,u,x,y,z \in Z_p,B \in E,s > u, x > z$
公開鍵：$A,C,D,E \in E,D=A^sB^tC^u,E=A^xB^yC^z$
$p,q,r \in Z_p$ の異なる原始元。
$P,Q,R \in E$　は同じ楕円曲線上の異なる位数の点。
つまり、半直積に使う元、$A,B,C$は、それぞれ有限体の元$p,q,r$と楕円曲線の異なる点$P,Q,R$とを任意に取り、
$A=(p,P),B=(q,Q),C=(r,R)$
である。

$(c,s)^n=(c^n,\frac{c^{n-1}-1}{c-1}s)$

であるから、公開鍵を$D,E$とすると、
$D=(p,P)^s+(q,Q)^t+(r,R)^u=(p^s+q^t+r^u,\frac{p^{s-1}-1}{p-1}P+\frac{q^{t-1}-1}{q-1}Q+\frac{r^{u-1}-1}{r-1}R)$
$E=(p,P)^x+(p,P)^s+(q,Q)^t+(r,R)^u+(r,R)^y=(p^x+p^{s}+q^t+r^{u}+r^y,(\frac{p^{x-1}-1}{p-1}+\frac{p^{s-1}-1}{p-1})P+\frac{q^{t-1}-1}{q-1}Q+(\frac{q^{y-1}-1}{q-1}+\frac{r^{u-1}-1}{r-1})R)$

暗号文：
$C1=A^r+D+C^{r'}=A^{r}+A^s+B^t+C^u+C^{r'}$
$C2=A^{r}+E+C^{r'}=A^r+A^x+A^s+B^t+C^{r'}+C^y+C^u$

復号
アリスは$x,y$を知っているので以下を計算できる。
$X=A^{(x)}+C1+C^{(y)}=A^{s}+A^x+A^r+B^t+C^{r'}+C^y+C^u=C2$

# 問題
**$Z=A^s+B^t+C^u$から指数$s,t,u$を求めよ。**
例：
$1575=(7^x+11^y+13^z) \bmod 2039$
これなら$x,y,z$はどうなりますか？

ただし、$A=(p,P),B=(q,Q),C=(r,R)$なる左側要素を素体の元、右側要素を素体上定義された楕円曲線の点であるような半直積とし、$B$は秘密であるとする。

ヒント：平文と暗号文のペアがいくらでも得られる状態で、ガウスの消去法が使えるかどうか。

# 解読法（以下の記事より既出ですが）

https://qiita.com/fumumue/items/55b61f63a005f290a2c6

今、加法的多元離散対数問題を解読しようとする。

$D=(p,P)^s+(q,Q)^t+(r,R)^u=(p^s+q^t+r^u,\frac{p^{s-1}-1}{p-1}P+\frac{q^{t-1}-1}{q-1}Q+\frac{r^{u-1}-1}{r-1}R)$

このように書けるところから、異なる原始根に対するべき乗の線形結合が線形代数の手法で簡単に破られるかもしれないという潜在的な危険性がある。つまり$A,B,C$に関するベクトル空間があったときに、$D$が線形独立であるか従属であるかという問題と、ベクトルとして考えたときにガウスの消去法のような手法を使って簡単に解けるかもしれないという可能性です。まあ、隠すべきは冪指数なので係数を求めるような方法では簡単に解けないと思うのですが、それでも例えば$A^s=(p,P)^s=y$のとき、$log(y)=s(log(A))$として指数$s$を線形代数の手法で解けるかもしれないという危険性です。いくら加法で誤魔化したつもりでも、代数的構造が線形では単純すぎて簡単に解読できるかもしれないということです。しかし今の時点では、$A,B,C$の個々の値は分かっていても、その真数$y$が分からない状態なので、できるかもしれないしできないかもしれない。それは暗号解析に当たって、条件を満たすような暗号文や、選択平文の関係から導き出されるものなので、解読できる可能性が大きいと思います。このように１つの鍵に対する安全性は確立できても、暗号化したデータにはその保証がないというべきでしょう。幸いなのは$B$が秘密であることです。１つでもわからない値があればLWEのようなエラーが入った多元１次連立多項式の回を計算する問題にうまく関連付けることができるかもしれない。（この辺り何も理解しないで考えているというのがわかります・・・）

今のところは鍵交換に使っていますが、ここで今後公開鍵暗号方式を設計するときに必要なことは、線形代数に気をつけることと、LWE問題のような（未知の値はエラーが入って０になったと考えればいいので）別の問題に帰着できるようにすることです。
（考察中）


# 20230623

MDLPに関する記述に疑問を持って、半直積に加法を定義してみました。
すると魔法のように難しい問題になって６４ビット整数なら４つ秘密指数を知らないと解けない暗号になりました。
実装はまだです。

# 20230620
楕円曲線の半直積でできた多元離散対数問題に基づく公開鍵暗号と分割問題に基づく暗号（いずれもオリジナル）を実装しました。  
名前は、「離散対数とハードボイルドワンダーランド」に決めましたｗ  
ec.cppのehw関数がそれです。  
この方式は勢いと思いつきで作ったのですが、幸運なことに量子計算機で解読する方法がまだ見つかっていないそうです。  

新型暗号はもう出来てるのでec.cppの中に混在しています。  
これは謎のようなものですが（エニグマ？）、まだやりたいことがあるのでchatGPTを使って説明を穴埋めしたいと思います。  
つまり、MDLPとDecomposition Problemを使った暗号が混在しているということです。  
いまは巨大整数ライブラリであるNTLを使っていますが、これもGPTを使ってCのコードに置き換えていきたい。  
それともRustがいいのか、C++のままでいいのか・・・（迷ってます）  

詳しくは以下のサイトをご覧ください。

https://qiita.com/fumumue/items/5f622ee6ea83cbd4a7f9

# 20230605
## 警告
この方式で行列を使うと線形代数攻撃で解読できます。
なぜなら、A1,A2で同じ元を使っている２つの式を合わせれば、未知変数の個数と一致して、全ての秘密鍵が計算できるからです。それに対抗するためには、
まだどのような組み合わせが安全かわかりませんが、線形代数以外の群を使った半直積を試してみてください。
例えば、x,yを楕円曲線の点を成分に持つ行列として、それ以外の$a,b,c,d$を整数行列にするなど、線形代数が使えないようにした変な行列で試してください。
問題は行列以外の群に何を選ぶかです。もしそれが行列でないなら、それを使って安全な鍵交換ができるはずです。

## 行列以外の群に対する鍵交換
（とは言え行列以外の非可換軍って少ないのよね・・・。）

ここではブレイド群を含めた一般の非可換群に対する鍵交換プロトコルを紹介します。
この問題は、私が離散対数でもなく共役元探索問題でもない、非可換群を用いた鍵交換で初めて経験した方法です。
世の中非可換群を使ったプロトコルなんて山のようにあるけど、本当は可換になる部分群を使ってたりしてがっかりすることが多いのですが、そんな偽物ではなく本当に非可換な元同士を組み合わせて鍵交換をしようというのが動機です。
行列を使った暗号は色々攻撃法があって危険かもしれませんが、そのときは半直積にしてお好きな群を非可換にしてみるのも手だと思います。
ちなみに置換群は行列なので解読できる可能性が高いです。
今度記事を書くときには、置換群と数ベクトルの半直積を作って、その弱点を調べたいと思います。
今まで置換群で共役元探索問題をやろうとして線形代数攻撃に悩まされてきたのですが、今度こそその攻撃を交わすことができればいいと思います。
ちなみに行列の３分割問題への攻撃法は先程思いつきましたので、整数行列は絶対に使わないでください。
多変数多項式を要素に持つベクトルと、整数ベクトルの半直積などをやってみたいです。
例えば、１変数多項式をｎ本持つベクトルと、ｎこの整数を持つベクトルとの半直積を考える。
ベクトルを$v,x$、多項式を$f,g$とすると$(v,f)(x,g)=(f_n(x_n)+v_n,f_ng_n)$（ここで$Z/mZ$は整数環,$I(x)$は多項式の法となる既約多項式）のような演算を定義します。

# Decomposition Problem

Problem 2.(Decomposition Problem)とは、 
ある$v = x^{-1}a_2x_2,a$が与えられた時、$ , x1 ∈ H, a ∈ A, x2 ∈ H'.$
（要するに$x1,x2$が冪等元でないこと）
を満たすような$x_1,x_2$を求める問題です。

ここでは3分割のうち1つが公開されている問題になっているはずです。
見た感じ行列を使った暗号によく使われるようです。
ちょっと線形代数攻撃を試してみましょう。
今$a$が公開されていて$x_1,x_2$が秘密であり、
$v=x_1ax_2$
であったとしましょう。そのとき、未知の元$x_2$の逆元を右からかけると、
$vx_2^{-1}=x_1a$
この条件をみたすような$x_1,x_2^{-1}$を計算する問題がこの暗号の強度になります。

次にまず、アリスは次のように非可換群の元を取ります。
例えばｎ時の対称群$S_n$にします。

アリスは6個の異なる元を取ります。
$a,b,c,d,x,y \in S_n$
ボブも同様に６個の元を取ります。
アリスはそのうち$x,y$だけを公開し、アリスとボブとの間で共有します。
公開する鍵はボブが決めても問題ありません。
つまりボブの秘密鍵は、
$e,f,g,h' \in S_n$
の４つだけです。

# 鍵設定
アリスは次のようにして公開鍵を作ります。
まず$d^{-1}a=h'^{-1}e=\phi$であるように決めます。
これは後で述べるビットパターンの分離記号としての役割を果たす元です。

$A1=(a_1=axb,a_2=b^{-1}xc,a_3=c^{-1}xd^{-1})$
$A2=(b_1=ayb,b_2=b^{-1}yc,b_3=c^{-1}yd^{-1})$

$B1=(c_1=exf,c_2=f^{-1}xg,c_3=g^{-1}xh'^{-1})$
$B2=(d_1=eyf,d_2=f^{-1}yg,d_3=g^{-1}y h'^{-1})$

$A1,A2$はアリスの公開鍵、$B1,B2$がボブの公開鍵、$a,b,c,d$がアリスの秘密鍵、$e,f,g,h'$がボブの秘密鍵です。

# ビットパターンと鍵共有
（ややこしいのでじっくり読みましょうｗ）

ここで1つ制約条件をつけます。
それはアリスとボブの秘密のバイナリ列の長さが偶数であるという仮定です。
その仮定のもとにこの鍵交換は成り立ちます。
ビットパターンを
$h=(0,0),i=(0,1),j=(1,0),k=(1,1)$とします。
長さ2のバイナリ列にはこの4パターンしかないので、これだけ定義できれば十分です。

この時、各2ビット列が、アリストボブの間でどのように組み合わせられるのか見ていきます。
まず、パターンhのとき、アリスの鍵の組み合わせは、
$h=a_1a_2a_3=ax^3d^{-1}$と決めます。
同様にパターンiの場合は、
$i=a_1b_2a_3=axyxd^{-1}$
$j=b_1a_2b_3=ayxyd^{-1}$
$k=b_1b_2b_3=ay^3d^{-1}$

となるように決めます。
同様にボブも、
$h1=c_1c_2c_3=ex^3h'^{-1}$と決めます。
同様にパターンiの場合は、
$i1=c_1d_2c_3=exyxh'^{-1}$
$j1=d_1c_2d_3=eyxyh'^{-1}$
$k1=d_1d_2d_3=ey^3h'^{-1}$

これは行ってしまえば、０と１を2つの元の対応関係で表現しているということです。
3つの異なる元を組み合わせると同時に、ビット列を共有したいという要求を実現できます。

ここで$d^{-1}a=h^{-1}e=\phi$と決めた事を思い出してください。
あとはパターン通りに鍵の組み合わせを並べるだけです。

アリスのビットパターンが1010のときは、
$K_{a_1}=jj=b_1a_2b_3b_1a_2b_3=ayxy\phi yxyd^{-1}$
$K_{a_2}=j1j1=c_1d_2c_1b_1d_2b_3=eyxy\phi yxyh'^{-1}$
1111の場合は、
$K_{b_1}=kk=b_1b_2b_3b_1b_2b_3=ay^3\phi y^3d^{-1}$
$K_{b_2}=k1k1=ey^3\phi y^3h'^{-1}$

$K_{alice}=K_{a_1}K_{b_1}=ayxy\phi yxy^4\phi y^3d^{-1}$
$K_{bob}=K_{a_2}K_{b_2}=eyxy\phi yxy^4\phi y^3h'^{-1}$

であり、鍵に割り当てられた番号同士をつなぎ合わせて、$K_{alice},K_{bob}$からそれぞれアリスとボブの秘密鍵を取り除けば鍵共有は出来上がりです。
これは非可換でなければならないという要請に答えるための鍵共有ができる元の3分割問題(triple decomposition problem)を利用したものとなると思います。（不安）

ここで問題が。
$\phi$が公開されることで、特定の位置に$\phi$があることがわかるというのはどの程度問題になるだろうか？
かと言って1にしてしまうと3つのパーツを全部揃えれば線形代数攻撃に弱くなるし、何かいい方法があるかもう少し考えます。

成功したら大発見です。

