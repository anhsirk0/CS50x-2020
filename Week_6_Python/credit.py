# Luhn's Algorith to check credit card number

# num = 4003600000000014

##### sum of digits #####
def digitsSum(n):
    n_string = str(n)
    c = 0
    for i in range(len(n_string)):
        c += int(n_string[i])
    return c

###### check validity of card number #####


def validateCard(card_num):
    ## card num as string ##
    num_string = str(card_num)
    l = len(num_string)
    sw = int(num_string[:2])
    count1, count2 = 0, 0
    if l == 14 or l < 13 or l > 16:
        print("INVALID")
        return
    for i in range(- l, 0):
        ## every other digits ##
        if i % 2 == 0:
            x = int(num_string[i]) * 2
            count1 += digitsSum(x)
        ## rest of the digits ##
        else:
            count2 += int(num_string[i])

    total = count1 + count2
    if total % 10 == 0:
        if l == 15 and sw in [34, 37]:
            print("AMEX")
        elif int(sw / 10) == 4 and l in [13, 16]:
            print("VISA")
        elif 50 < sw < 56 and l == 16:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


num = int(input("Number: "))
validateCard(num)
