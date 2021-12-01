Rcon = [0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36]
s_box = [0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,\
         0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,\
         0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,\
         0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,\
         0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,\
         0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,\
         0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,\
         0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,\
         0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,\
         0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,\
         0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,\
         0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,\
         0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,\
         0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,\
         0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,\
         0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16]
forward_mix_matrix = [[0x02,0x03,0x01,0x01],[0x01,0x02,0x03,0x01],[0x01,0x01,0x02,0x03],[0x03,0x01,0x01,0x02]]

def key_expansion(key):
    i = 0
    temp = [0,0,0,0]

    round_key = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],\
                [0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
    for i in range(4):
        j=0
        for j in range(4):
            round_key[i][j] = key[4*i+j]
    for i in range(4,44):
        for j in range(4):
            temp[j] = round_key[i-1][j]
        if (i % 4 == 0):
            temp = key_sub(key_rotate(temp))
            temp[0] = temp[0] ^ Rcon[i//4-1]
        for j in range(4):
            round_key[i][j] = round_key[i-4][j] ^ temp[j]
    return round_key
    
def key_rotate(temp):
    t = temp[0]
    for i in range(1,4):
        temp[i-1] = temp[i]
    temp[3] = t
    return temp
    
def key_sub(temp):
    for i in range(4):
        temp[i] = s_box[temp[i]]
    return temp

def encrypt_sub(state):
    for i in range(4):
        for j in range(4):
            state[i][j] = s_box[state[i][j]]
    return state
    
def shift_row(state):
    for i in range(4):
        if (i==1):
            t = state[1][0]
            for j in range(1,4):
                state[1][j-1] = state[1][j]
            state[1][3] = t
        if (i==2):
            t1 = state[2][0]
            t2 = state[2][1]
            for j in range(2,4):
                state[2][j-2] = state[2][j]
            state[2][2] = t1
            state[2][3] = t2
        if (i==3):
            t1 = state[3][0]
            t2 = state[3][1]
            t3 = state[3][2]
            state[3][0] = state[3][3]
            state[3][1] = t1
            state[3][2] = t2
            state[3][3] = t3
    return state

def mix_column(a,state):
    res = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
    for i in range(4):
        for j in range(4):
            for  k in range(4):
                res[i][j] = res[i][j] ^ mul(a[i][k],state[k][j])
    return res

def mul(a,b):
    product = 0
    for num in range(8):
        if b & 1 == 1:
            product ^= a
        highbit = a & 0x80
        a <<= 1
        if highbit == 0x80:
            a ^= 0x1b
        b >>= 1
    return product % 256

def add_round_key(state,key_state):
    for i in range(4):
        for j in range(4):
            state[i][j] = state[i][j] ^ key_state[i][j]
    return state

def aes_encrypt(plain_text,round_key):
    state = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
    key_state = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
    for j in range(4):
        for i in range(4):
            state[i][j]=plain_text[j*4+i]
    for i in range(11):
        for j in range(4):
            for k in range(4):
                key_state[k][j] = round_key[i*4+j][k]
        if (i==0):
            state = add_round_key(state,key_state)
        elif (i==10):
            state = shift_row(encrypt_sub(state))
            state = add_round_key(state,key_state)
        else:
            state = shift_row(encrypt_sub(state))
            state = mix_column(forward_mix_matrix,state)
            state = add_round_key(state,key_state)
    res = []
    for i in range(4):
        for j in range(4):
            res.append(state[j][i])
    return res

# main
mode = input()
blocksize = int(input())

if(mode == "CBC"):
    # input process
    k = input()
    iv = input()
    pt = input()

    key = []
    IV = []
    plain_text = [[0 for col in range(16)] for row in range(blocksize)]

    for i in range(blocksize):
        for j in range(16):
            plain_text[i][j] = int(pt[i*32 + j*2 : i*32 + j*2 + 2], 16)
    
    # key, iv, plaintext to int
    for i in range(16):
        key.append(int(k[i*2:i*2+2], 16))
        IV.append(int(iv[i*2:i*2+2], 16))
        plain_text[0][i] = plain_text[0][i] ^ IV[i]

    # key expansion, first block process
    round_key = key_expansion(key)
    tmp = aes_encrypt(plain_text[0], round_key)

    answer = ""
    for i in range(16):
        if(tmp[i] < 16):
            answer = answer + "0" + hex(tmp[i])[2:]
        else:
            answer = answer + hex(tmp[i])[2:]

    # if blocksize is 1 just return
    if blocksize < 2:
        print(answer)
    else:
        # plaintext xor ciphertext
        for i in range(1, blocksize):
            for j in range(16):
                plain_text[i][j] = plain_text[i][j] ^ tmp[j]
            
            #put the answer after aes encryption
            tmp = aes_encrypt(plain_text[i], round_key)
            for j in range(16):
                if(tmp[j] < 16):
                    answer = answer + "0" + hex(tmp[j])[2:]
                else:
                    answer = answer + hex(tmp[j])[2:]

            
        print(answer)
else:
    #input process
    k = input()
    ctr = input()
    pt = input()

    key = []
    Counter = []
    plain_text = [[0 for col in range(16)] for row in range(blocksize)]

    for i in range(blocksize):
        for j in range(16):
            plain_text[i][j] = int(pt[i*32 + j*2 : i*32 + j*2 + 2], 16)
    
    #key, counter to int & key expansion
    for i in range(16):
        key.append(int(k[i*2:i*2+2], 16))
        Counter.append(int(ctr[i*2:i*2+2], 16))
    round_key = key_expansion(key)
    
    # all block process
    idx = 15
    answer = ""
    
    for i in range(blocksize):
        tmp = aes_encrypt(Counter, round_key)
        for j in range(16):
            tmp[j] = tmp[j] ^ plain_text[i][j]
        if(Counter[idx] < 255):
            Counter[idx] += 1
        else:
            idx -= 1
            Counter[idx] += 1

        for j in range(16):
            if(tmp[j] < 16):
                answer = answer + "0" + hex(tmp[j])[2:]
            else:
                answer = answer + hex(tmp[j])[2:]
    print(answer)
        

# cbc 모드
# 1. plain_text 첫 블록과 iv를 xor한다.
# 2. aes_encryption을 거쳐 나온 암호문을 두 번째 블록과 xor한다.
# 3. 역시 aes_encryption을 거쳐 나온 암호문을 n+1번째 블록과 xor한다.

# ctr 모드
# 1. counter를 aes_encryption을 거친다.
# 2. 나온 암호문을 plain_text 첫 블록과 xor한다.
# 3. counter+1를 aes_encryption을 거친다.
# 4. 마찬가지로 plain_text 두 번째 블록과 xor한다.