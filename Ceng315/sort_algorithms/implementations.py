def insertion_sort(Array, length):
    """
        A basic implementation of insertion sort
    """
    for j in range(1, length):
        key = Array[j]
        i = j - 1
        while i >= 0 and Array[i] > key:
            Array[i + 1] = Array[i]
            i -= 1
        Array[i + 1] = key
    return Array

def merge(Array, length1, length2):
    """
        Helper function for Merge sort
    """
    idx1 = 0
    idx2 = 0
    sorted_idx = 0
    sorted_arr = []
    for sorted_idx in range(length1 + length2):
        if idx1 == length1:
            while idx2 < length2:
                sorted_arr.append(Array[length1 + idx2])
                idx2 += 1
            return sorted_arr
        if idx2 == length2:
            while idx1 < length1:
                sorted_arr.append(Array[idx1])
                idx1 += 1
            return sorted_arr
        if Array[idx1] > Array[length1 + idx2]:
            sorted_arr.append(Array[length1 + idx2])
            idx2 += 1
        else:
            sorted_arr.append(Array[idx1])
            idx1 += 1
    return sorted_arr

def merge_sort(Array, length):
    """
        A basic implementation for merge sort.
    """
    if length == 1:
        return Array
    else:
        sub_arr1 = merge_sort(Array[0:(length // 2)], length // 2)
        sub_arr2 = merge_sort(Array[(length // 2):length], length - length // 2)
        return merge(sub_arr1 + sub_arr2, length // 2, length - length // 2)

def bubble_sort(Array):
    """
        it is the simplest sorting algorithm to implement.
        Here it is.
        A basic implementation of bubble sort.
    """
    sorted_flag = True
    length = len(Array)
    while sorted_flag:
        sorted_flag = False
        for i in range(length - 1):
            if Array[i + 1] < Array[i]:
                Array[i], Array[i  + 1] = Array[i + 1], Array[i]
                sorted_flag = True
    return Array

def Partition(Array, begin_idx, end_idx):
    """
        Helper function for quick sort.
    """
    pivot = Array[begin_idx]
    i = begin_idx 
    for j in range(begin_idx + 1, end_idx + 1):
        if Array[j] <= pivot:
            i += 1
            Array[i], Array[j] = Array[j], Array[i]
    Array[begin_idx], Array[i] = Array[i], Array[begin_idx]
    return i

def quick_sort(Array, begin_idx, end_idx):
    """
        A basic implementation of quick sort.
    """
    if begin_idx < end_idx:
        q = Partition(Array, begin_idx, end_idx)
        quick_sort(Array, begin_idx, q - 1)
        quick_sort(Array, q  + 1, end_idx)
    return Array

def counting_sort(Array, length, range_tuple):
    """
        A basic implementation of counting sort
        It makes sense when used with Radix Sort.
    """
    lower_bound = range_tuple[0]
    upper_bound = range_tuple[1]
    Auxiliary_storage = [ 0 ] * (upper_bound - lower_bound + 1)
    for i in range(length):
        Auxiliary_storage[Array[i]] += 1
    
    for i in range(1, upper_bound - lower_bound + 1):
        Auxiliary_storage[i] += Auxiliary_storage[i - 1]
    sorted_array = [None] * length
    for i in range(length - 1, -1, -1):
        sorted_array[Auxiliary_storage[Array[i]] - 1] = Array[i]
        Auxiliary_storage[Array[i]] -= 1
    return sorted_array 
    

def radix_sort(sorting_function = counting_sort, *args, **kwargs):
    """
        A fantasy implementation of radix sort.
        Although, it is to be used with counting sort,
            It can be used with all sorting algorithm as long as algorithm guarentees 
            that it will preserve the order when elements are equal.
                e.g. it is not compatibe with heap sort
                    since it changes the order.
        Since it is more complex than it needed to be, It may, actually will,
            take more time to sort big size of arrays than any normal implementation.
        Use as: 
            radix_sort(counting_sort, array, len(array), (0,9))
            radix_sort(quick_sort, array, 0, len(array) - 1)
    """
    class digit_number:
        """
            An interface that provides the numbers to act like digit arrays.
            In first design, overloading comparison functions was needed; 
                however, this one does not require.
                But I kept them.
                I like them.
        """
        _curr_digit = -1
        def __init__(self, number):
            self.digits = []
            while number:
                self.digits.insert(0, number % 10)
                number //= 10
        def __getattr__(self, attr):
            if hasattr(self, attr):
                return self.attr
            elif hasattr(self.digits, attr):
                return self.digits.__getattr__(attr)
            elif type(attr) is int:
                return self[attr]
            else:
                raise AttributeError("{} has no attr as {}".format(type(self.digits), attr))
        def __getitem__(self, idx):
            return self.digits[idx]
        def __eq__(self, other_digit):
            return self[_curr_digit] == other_digit[_curr_digit]
        def __gt__(self, other_digit):
             return self[_curr_digit] > other_digit[_curr_digit]
        def __ge__(self, other_digit):
            return self > other_digit or self == other_digit 
        def __lt__(self, other_digit):
            return not (self >= other_digit)     
        def __le__(self, other_digit):
            return not (self > other_digit)
        def __ne__(self, other_digit):
            return not (self == other_digit)
        def __str__(self):
            num_idx = 0
            length = len(self.digits)
            while num_idx < length - 1 and self[num_idx] ==0:
                num_idx+=1
            return ''.join([str(x) for x in self[num_idx:]])
        def __int__(self):
            return int(str(self))

        def rearrange(self, digit_count):
            dc = len(self.digits)
            for _ in range(digit_count - dc):
                self.digits.insert(0, 0)
         
        def get_curr_digit(self):
            return self[digit_number._curr_digit]
        
        def increment_digit():
            digit_number._curr_digit -= 1
        
    class array_as_digits:
        """
            An interface that uses digit_numbers and creates  
                certain digit's arrays. 
            also includes sort implementation.
        """
        _curr_digit_array = []
        incr = digit_number.increment_digit
        main_sorting_function = None
        _original_array = None
        class tuple_n_idx:
            """
                An interface, again, to use (number, index) arrays
                    as number arrays.
                Overloading the comparison functions, which brokes my heart,
                    made possible act it like number.
            """
            def __init__(self, number, idx):
                self._tp = (number, idx)
            def __gt__(self, rhs):
                return self[0] > rhs[0]
            def __lt__(self, rhs):
                return self[0] < rhs[0]
            def __ge__(self, rhs):
                return self[0] >= rhs[0]
            def __le__(self, rhs):
                return self[0] <= rhs[0]
            def __eq__(self, rhs):
                return self[0] == rhs[0]
            def __ne__(self, rhs):
                return self[0] == rhs[0]
            def __index__(self):
                return self[0]
            def __add__(self, rhs):
                self[0] += rhs[0]
                return self
            def __mul__(self, rhs):
                self[0] *= rhs[0]
                return self
            def __truediv__(self, rhs):
                self[0] /= rhs[0]
                return self
            def __sub__(self, rhs):
                self[0] -= rhs[0]
                return self
            def __getitem__(self, idx):
                return self._tp[idx]
            def __str__(self):
                return str(self[0])
            def __int__(self):
                return int(self[0])
            
        def __init__(self, sorting_function, array, *args, **kwargs):
            array_as_digits._original_array = array
            self.Array_as_digits = []
            array_as_digits.max_arr_length = 0
            array_as_digits.main_sorting_function = sorting_function
            array_as_digits._curr_digit_array = [None] * len(array)
            array_as_digits.args = args
            array_as_digits.kwargs = kwargs
            for idx, number in enumerate(array):
                self.Array_as_digits.append(digit_number(number))
                if array_as_digits.max_arr_length < len(self.Array_as_digits[-1].digits):
                    array_as_digits.max_arr_length = len(self.Array_as_digits[-1].digits)
            for number in self.Array_as_digits:
                number.rearrange(array_as_digits.max_arr_length)
        def __str__(self):
            res = ''
            res +='['
            for item in self.Array_as_digits[:-1]:
                res += str(item) + ', '
            res += str(self.Array_as_digits[-1]) + ']\n'
            return res
        
        def get_digit_array(self):
            for idx, n in enumerate(self.Array_as_digits):
                array_as_digits._curr_digit_array[idx] = array_as_digits.tuple_n_idx(n.get_curr_digit(), idx)
            array_as_digits.incr()
        
        
        def sort(self):
            length = len(array_as_digits._original_array)
            sorted_arr = None
            result = [0] * length
            for i in range(self.max_arr_length):
                self.get_digit_array()   
                sorted_arr = array_as_digits.main_sorting_function(array_as_digits._curr_digit_array, *array_as_digits.args, **array_as_digits.kwargs)
                
                self.Array_as_digits = [self.Array_as_digits[idx] for _, idx in sorted_arr]
                
            return [int(number) for number in self.Array_as_digits]


    arr = array_as_digits(sorting_function, *args, **kwargs)
    return arr.sort()
    

def heap_sort(Array, is_max = True):
    """
        A basic heap sort implementation.
        Includes a max_heap class,
            Creates a max heap, and sort the array
            with the help of max heap.
        If it is asked to sort in ascending order, is_max must set to True,
            if not, is_max must be False.
            It is responsible for just,
            if max element will inserted to end or beginning.
    """
    class max_heap:
        """
            An interface that provides a max heap implementation and usage.
        """
        def __init__(self, array, max_size = 100):
            self.size = 0

            self.array = [None] * max_size
            for item in array:
                self.insert(item)
        
        def insert(self, item):
            self.array[0] = item
            if self.size + 1 == len(self.array):
                self.array += [None] * (len(self.array) + 1)
            self.size += 1
            hole = self.size
            while item > self.array[hole // 2]:
                self.array[hole] = self.array[hole//2]
                hole //= 2
            self.array[hole] = item
        
        def pop_max(self):
            if self.array:
                max_number = self.array[1]
                self.array[1] = self.array[self.size]
                self.size -= 1
                self.percolateDown(1)
                return max_number
            else:
                raise Exception('Empty List!')
            
        def percolateDown(self, hole):
            item = self.array[hole]
            while hole * 2 <= self.size:
                child = hole * 2
                if child != self.size and self.array[child] < self.array[child + 1]:
                    child += 1
                if self.array[child] > item:
                    self.array[hole] = self.array[child]
                else:
                    break
                hole = child
            self.array[hole] = item
        
        def __bool__(self):
            return self.size > 0

    heap = max_heap(Array, len(Array) + 1)
    sorted = []
    while heap:
        if is_max:
            sorted.append(heap.pop_max())
        else:
            sorted.insert(0, heap.pop_max())
    return sorted



if __name__ == '__main__':
    A = [3, 4, 1, 2, 7, 8, 3, 0, 11, 12, 5, 9, 6]
    B = [3, 4, 1, 2, 7, 8, 3, 3, 3, 3, 5, 9, 6] 
    C = [345, 457, 112, 233, 757, 8, 10, 0, 11, 12, 5, 9, 22222222] 
    D = [123, 222225, 344, 457, 112, 233, 757, 8, 10, 0, 11, 12, 5, 9, 6] 
    print(A)
    quick_sort(A, 0, len(A) - 1)
    print(B)
    print(counting_sort(B, len(A), (0, 12)))
    print(C)
    print(radix_sort(counting_sort, C, len(C), (0,9))) #paramaters required by sorting function must be added here
    print(D)
    print(heap_sort(D, False))
