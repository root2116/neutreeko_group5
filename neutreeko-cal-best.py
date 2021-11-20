#グラフの読みこみ
f = open("newtext1", mode = "r")

g1 = []
c = 0

for i in f:
    x = i.split()
    for j in range(len(x)):
        x[j] = int(x[j])

    g1.append(x)
    c += 1
    if (c%100000 == 0):
        print(c)
f.close()

print("fin1")

#グラフ（反転ver）の構築

g2 = [[] for i in range(len(g1))]
for i in range(len(g1)):
    if (i%100000 == 0):
        print(i)
    for j in g1[i]:
        g2[j].append(i)

print("fin2")


#condの読み込み
f = open("cond", mode = "r")


for i in f:
    cond = i.split()
    for j in range(len(cond)):
        cond[j] = int(cond[j])

f.close()

print("fin cond")


#tbl[i][0]:状態iの遷移先（手番は相手）のうち（相手目線で）負けの状態の数
#(1個でもあれば状態iは勝ち盤面)
#tbl[i][1]:状態iの遷移先（手番は相手）のうち（相手目線で）引き分けの状態の数
#tbl[i][2]:状態iの遷移先（手番は相手）のうち（相手目線で）勝ちの状態の数
#(遷移先全部がこれだと状態iは負け盤面)

tbl = [[0,0,0] for i in range(len(g2))]

#最長何手で決着がつくか（つかない場合形式的に0を入れる）
mate = [0 for i in range(len(g2))]

print("fintbl")

#勝敗の決まった盤面を例の整数の形で入れる
nextcheck = []
for i in range(len(g2)):
    if (cond[i] != 0):
        nextcheck.append(i)


#繰り返し回数を持つ
layer = 1


c = 0#進捗確認用（気にしないで）

#盤面の勝敗を判定
while(len(nextcheck) > 0):
    c += len(nextcheck)#進捗確認用（気にしないで）
    print(c)#進捗確認用（気にしないで）

    #nowcheckに入っている盤面（勝敗確定済み）に1手でたどり着く盤面について
    #勝敗を判定し、新たに勝敗が確定した盤面をnextcheckに放り込む
    nowcheck = nextcheck
    nextcheck = []
    for i in nowcheck:
        for j in g2[i]:
            if (cond[j] == 0):
                tbl[j][cond[i] + 1] += 1
                if (tbl[j][0] > 0):
                    cond[j] = 1
                    nextcheck.append(j)
                    mate[j] = layer

                if (tbl[j][2] == len(g1[j])):
                    cond[j] = -1
                    nextcheck.append(j)
                    mate[j] = -layer
    layer += 1

print("fincal")


#最善手を計算
best = [0 for i in range(len(g2))]
for i in range(len(best)):
    #勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移
    if (cond[i] == 1):
        nexthand = 0
        nowmate = -10**10
        for j in g1[i]:
            if (cond[j] == -1):
                if (mate[j] > nowmate):
                    nowmate = mate[j]
                    nexthand = j
        best[i] = nexthand

    #負け盤面なら遷移先の（相手目線の）勝ち盤面で決着手数の長いものに遷移
    elif (cond[i] == -1):
        nexthand = 0
        nowmate = -10**10
        for j in g1[i]:
            if (cond[j] == 1):
                if (mate[j] > nowmate):
                    nowmate = mate[j]
                    nexthand = j
            else:
                print("err1")
        best[i] = nexthand

    #引き分け盤面なら遷移先の（相手目線の）引き分け盤面に適当に遷移
    else:
        for j in g1[i]:
            if (cond[j] == 0):
                nexthand = j
                break
            elif (cond[j] == -1):
                print("err2")
        best[i] = nexthand
print("finbest")

#最善手と手数をテキストに保存

f = open("best1121", mode = "w")

for i in range(len(best)):
    best[i] = str(best[i])

x = " ".join(best)
f.write(x)
f.write("\n")


f.close()

print("finbest-o")

f = open("mate1121", mode = "w")

best = mate

for i in range(len(best)):
    best[i] = str(best[i])

x = " ".join(best)
f.write(x)
f.write("\n")


f.close()

print("finmate-o")

