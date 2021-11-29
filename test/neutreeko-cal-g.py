
#配列を整数（例の12桁）に変換
def to_int(tbl):
    ans = 0
    for i in range(6):
        ans += tbl[i]*10**(2*i)
    return(ans)

#勝敗を判定
def linecheck(tbl):
    x = tbl[0]
    y = tbl[1]
    z = tbl[2]
    f1 = 0
    sa1 = y - x
    sa2 = z - y
    if (sa1 == sa2):
        if (sa1 == 1):
            if (x//5 == y//5 == z//5):
                f1 = 1
        elif (sa1 == 5):
            f1 = 1
        elif (sa1 == 4 or sa1 == 6):
            if (x//5 + 2 ==y//5 + 1 == z//5):
                f1 = 1
    
    x = tbl[3]
    y = tbl[4]
    z = tbl[5]
    f2 = 0
    sa1 = y - x
    sa2 = z - y
    if (sa1 == sa2):
        if (sa1 == 1):
            if (x//5 == y//5 == z//5):
                f2 = 1
        elif (sa1 == 5):
            f2 = 1
        elif (sa1 == 4 or sa1 == 6):
            if (x//5 + 2 ==y//5 + 1 == z//5):
                f2 = 1
    return(f1, f2)

#tbl[i]を(dx,dy)方向に動かしたときの移動先（移動不可の場合0を返す）
#（次のnextboardと合わせて確認ください）
def move(tbl, i, dx, dy, n):
    x = tbl[i]//5
    y = tbl[i]%5

    nowx = x
    nowy = y
    while True:
        nextx = nowx + dx
        nexty = nowy + dy
        nextint = 5*nextx + nexty
        if (0 <= nextx < 5 and 0 <= nexty < 5 and nextint not in tbl):
            nowx = nextx
            nowy = nexty
        else:
            break

    if (x != nowx or y != nowy):
        nowint = nowx*5 + nowy
        tmp = []
        for j in range(6):
            tmp.append(tbl[j])
        tmp[i] = nowint
        tmp2 = []
        for j in range(3*n, 3*(n + 1)):
            tmp2.append(tmp[j])
        tmp2.sort()
        for j in range(3):
            tmp[j + 3*n] = tmp2[j]
        return(to_int(tmp))
    else:
        return(0)


#盤面(tbl)から遷移しうる盤面の計算
def nextboard(tbl, n):#n = 0 or 1
    ans = []
    for i in range(3*n, 3*(n + 1)):
        
        tmp = move(tbl, i, 1, 0, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, 1, 1, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, 0, 1, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, -1, 1, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, -1, 0, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, -1, -1, n)
        if (tmp != 0):
            ans.append(tmp)
            
        tmp = move(tbl, i, 0, -1, n)
        if (tmp != 0):
            ans.append(tmp)

        tmp = move(tbl, i, 1, -1, n)
        if (tmp != 0):
            ans.append(tmp)
    return(ans)


#盤面の出てきた順を保存
dic = {}

#グラフ（出てきた順に盤面が入っている）
#ただし、手番を区別したいため、それぞれ2こずつ入っている
#g[dic[盤面(例の整数)]*2]にその盤面から黒が動かしたときの遷移先が、
#g[dic[盤面(例の整数)]*2 + 1]にその盤面から白が動かしたときの遷移先が入る。
g = [[] for i in range(3540516*2)]

#勝敗を格納　（手版プレイヤー目線で勝ちなら1、まけなら-1）
cond = [0 for i in range(3540516*2)]

#グラフgの遷移の方向を反転したグラフ
#indexはgと同様
g2 = [[] for i in range(3540516*2)]


#dicの作成と勝敗の判定
i = 0
for i1 in range(25):
    for j1 in range(i1 + 1, 25):
        for k1 in range(j1 + 1, 25):
            for i2 in range(25):
                for j2 in range(i2 + 1, 25):
                    for k2 in range(j2 + 1, 25):
                        board = [i1, j1, k1, i2, j2, k2]
                        if (len(set(board)) == 6):
                            win1, win2 = linecheck(board)
                            if (win1&win2 == 0):
                                tmp = to_int(board)
                                dic[tmp] = i

                                cond[i] = win1 - win2
                                cond[i + 1] = win2 - win1
                                i += 2


#グラフの構築

c = 0#進捗確認用（気にしないで）

for i1 in range(25):
    for j1 in range(i1 + 1, 25):
        print(j1, c)#進捗確認用（気にしないで）
        for k1 in range(j1 + 1, 25):
            for i2 in range(25):
                for j2 in range(i2 + 1, 25):
                    for k2 in range(j2 + 1, 25):
                        board = [i1, j1, k1, i2, j2, k2]
                        now = to_int(board)
                        if (now in dic):
                            now = dic[now]
                            if (cond[now] == 0):
                                nex = nextboard(board, 0)
                                for i in nex:
                                    g[now].append(dic[i] + 1)
                                    g2[dic[i] + 1].append(now)
                                c += len(nex)

                                nex = nextboard(board, 1)
                                for i in nex:
                                    g[now + 1].append(dic[i])
                                    g2[dic[i]].append(now + 1)

                                 
                                
#グラフをテキストに保存

f = open("newtext1", mode = "w")

for i in range(len(g)):
    for j in range(len(g[i])):
        g[i][j] = str(g[i][j])

    x = " ".join(g[i])
    f.write(x)
    f.write("\n")


f.close()
print("fin1")


f = open("newtext2", mode = "w")

for i in range(len(g2)):
    for j in range(len(g2[i])):
        g2[i][j] = str(g2[i][j])

    x = " ".join(g2[i])
    f.write(x)
    f.write("\n")


f.close()



