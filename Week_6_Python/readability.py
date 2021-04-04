text = input("Text: ")


def count(text):
    letters = 0
    words = 1
    sentences = 0
    l = len(text)

    for i in range(l):
        t = text[i]
        if ('a' <= t <= 'z') or ('A' <= t <= 'Z'):
            letters += 1
        if t == " ":
            words += 1
        if t == '.' or t == '!' or t == '?':
            sentences += 1
    return([letters, words, sentences])


def grade(letters, words, sentences):
    l = (float(letters) / float(words)) * 100
    s = (float(sentences) / float(words)) * 100
    index = 0.0588 * l - 0.296 * s - 15.8
    return(round(index))


arr = count(text)
g = grade(arr[0], arr[1], arr[2])

if g < 1:
    print("Before Grade 1")
elif g > 16:
    print("Grade 16+")
else:
    print("Grade", g)
