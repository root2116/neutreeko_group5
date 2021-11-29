import itertools


num_list = list(range(1,26))
PATTERNS = 3542009 #25C3 * 22C3 = 3542000に近い素数

count_list = [0] * PATTERNS


for group1 in itertools.combinations(num_list,3):

    sub_num_list = [num for num in num_list if num not in group1]

    for group2 in itertools.combinations(sub_num_list,3):
        group = group1 + group2
        
        num = 0
        for i in range(6):
            num *= 100
            num += group[i]
            
       
        count_list[num % PATTERNS] += 1
        
max_count = max(count_list)
# print(count_list)

for i in range(max_count+1):
    print("{}: {}%".format(i,100*count_list.count(i)/sum(count_list)))
