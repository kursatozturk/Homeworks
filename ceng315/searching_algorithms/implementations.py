
def find_idx_of_rotation(Array):
    length = len(Array)
    begins = 0
    ends = length - 1
    first_elem = Array[0]
    idx = -1
    while ends - begins > 1:
        idx = begins + (ends - begins) // 2
        if Array[idx] > first_elem:
            begins = idx
        elif Array[idx] < first_elem:
            ends = idx
        else:
            break
    if ends == length -1:
        if Array[idx] > Array[ends]:
            return idx + 1
        else:
            return -1 
    else:
        return idx + 1
def regular_binary_search(Array, num):
    begins = 0
    ends = len(Array) - 1
    idx = (ends - begins) // 2

    while Array[idx] != num:
        idx = begins + (ends - begins) // 2
        if Array[idx] > num:
            ends = idx
        elif Array[idx] < num:
            begins = idx
    return idx

def binarySearch(Array, num):
    rot_idx = find_idx_of_rotation(Array)
    if rot_idx == -1:
        return regular_binary_search(Array, num)
    else:
        if num > Array[-1]:
            return regular_binary_search(Array[:rot_idx], num)
        else:
            return rot_idx +regular_binary_search(Array[rot_idx:], num)
if __name__=='__main__':
    from time import time
    print('Please provide size of array and rotation_factor: ')
    size_of_array = int(input())
    rotation_factor = int(input())

    arr = [x for x in range(rotation_factor, size_of_array)] + [y for y in range(rotation_factor)]

    search_num = int(input())
    begin_time = time()
    idx_num = binarySearch(arr, search_num)
    end_time = time()
    print("%d is found in %f second"%(idx_num, end_time - begin_time))
    print("searched number is {}, found {}".format(search_num, arr[idx_num]))
