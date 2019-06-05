Εργασία 1

Η παρούσα εργασία αποτελεί μία τροποποίηση του απλοϊκού webserver του project cnaiapi. Ακόμη τροποποιήθηκε η βιβλιοθήκη cnaiapi.h όπως και το make-linux. Τα exe αρχεία και οι βιβλιοθήκες δημιουργήθηκαν στο περιβάλλον Linux.  Η δουλειά του webserver της εργασίας είναι να εμφανίζει στον browser (cmd ή περιηγητή) του πελάτη (webclient) έναν link μέσω του οποίου γίνεται λήψη του video της σελίδας → http://users.uom.gr/~pfoul/net_test1.html . 

Κατά την εκκίνηση του, ο webserver αποθηκεύει τοπικά μέσα στον φάκελο make-linux την παραπάνω ιστοσελίδα στο αρχείο net_test1.html . Έπειτα γίνεται αναζήτηση εντός του αρχείου για το link του video (http://users.uom.gr/~pfoul/test_vid.mp4) . Όταν ο πελάτης επιχειρήσει να συνδεθεί στον webserver δημιουργείται το αρχείο video.html. Ανάλογα με το αίτημα του πελάτη το αρχείο έχει τις παρακάτω 3 μορφές: 
    • HOME PAGE: Εμφανίζει ώρα και το Link του video στον browser.
    • ERROR 404 PAGE: Εμφανίζει πως το αρχείο δεν βρέθηκε.
    • ERROR 400 PAGE: Εμφανίζει ότι το αίτημα δεν έγινε κατανοητό από τον server.

Πριν την εκτέλεση οποιοδήποτε executable απαιτείται η εκτέλεση της εντολής make apps μέσω του terminal των Linux εντός του φακέλου make-linux.
Για την εμφάνιση της HOME PAGE το path είναι το “/”. Για οποιοδήποτε άλλο path εμφανίζεται η σελίδα ERROR 404.
Για την εκκίνηση του webserver μέσω του τερματικού των Linux γίνεται εντός του φακέλου make-linux με την εντολή ./webserver <appnum> (π.χ. ./webserver 20000).
Για την εκκίνηση του webclient μέσω του τερματικού των Linux γίνεται εντός του φακέλου make-linux με την εντολή ./webclient <compname> <path> <appnum> (π.χ. ./webclient localhost / 20000), όπως επίσης και μέσω ενός browser γράφοντας την διεύθυνση http://localhost:<appnum> <path> (π.χ. http://localhost:20000/). 

Ονομ/νυμο: Ιακωβίδης Ευστάθιος
Α.Μ.: dai17168


