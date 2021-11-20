print("loading(標準15秒程度)")
dic = {}


def to_int(tbl):
    ans = 0
    for i in range(6):
        ans += tbl[i]*10**(2*i)
    return(ans)
"""
tbl = [3,7,24,5,7,13]
print(to_int(tbl))

"""


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

"""
tbl = [3,12,21,5,7,13]
print(move(tbl, 3, 1, 0, 1))
   """ 

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

    

"""
tbl = [3,12,21,5,7,13]
print(nextboard(tbl, 0))
"""


g = [[] for i in range(3540516*2)]
cond = [0 for i in range(3540516*2)]

g2 = [[] for i in range(3540516*2)]


calboard = []

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
                                calboard.append(tmp)
                                calboard.append(tmp)
                                cond[i] = win1 - win2
                                cond[i + 1] = win2 - win1
                                i += 2


print("fin cal-index")


f = open("mate1121", mode = "r")


for i in f:
    x = i.split()
    for j in range(len(x)):
        x[j] = int(x[j])

mate = x
f.close()

print("fin import1")


f = open("best1121", mode = "r")


for i in f:
    x = i.split()
    for j in range(len(x)):
        x[j] = int(x[j])

best = x
f.close()

print("fin import2")


def show(n):


    tbl = [[0,0,0,0,0] for i in range(5)]

    c = 0
    while (n > 0):
        y = n%100
        h = y//5
        w = y%5

        if (c < 3):
            tbl[h][w] = 1
        else:
            tbl[h][w] = 2
        c += 1
        n //= 100

    for i in range(5):
        print(tbl[i])

while True:
    print("盤面を入力してください（例、170301232107）")
    x = int(input())
    if (x in dic):
        print("この盤面から黒が動かすなら")

        show(x)
        print("↓↓↓↓↓")
        show(calboard[best[dic[x]]])

        if (mate[dic[x]] > 0):
            print("長くとも", mate[dic[x]], "手で勝ちます")
        if (mate[dic[x]] < 0):
            print("長くとも", -mate[dic[x]], "手で負けます")
        print("key", calboard[best[dic[x]]])

        print()
        print()

        print("この盤面から白が動かすなら")

        show(x)
        print("↓↓↓↓↓")
        show(calboard[best[dic[x] + 1]])

        if (mate[dic[x] + 1] > 0):
            print("長くとも", mate[dic[x] + 1], "手で勝ちます")
        if (mate[dic[x] + 1] < 0):
            print("長くとも", -mate[dic[x] + 1], "手で負けます")
        print("key", calboard[best[dic[x] + 1]])

        print()
        print()
        
    else:
        print("この盤面は辞書にありません。フォーマットを確認してください")




