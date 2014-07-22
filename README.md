Code is based on [Robert Sedgwich's talk](http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf "Left Leaning Red Black Tree") and its [java implementation](http://www.cs.princeton.edu/~rs/talks/LLRB/Java/).
It has invariant checking methods that were in the java implementation removed. 

Note: There is a bug in the remove() code. See Lee Stanza's left-leaning red-black tree implementation at 

http://www.teachsolaisgames.com/articles/balanced_left_leaning.html#datastructs

Note also this comment from http://www.read.seas.harvard.edu/~kohler/notes/llrb.html on the problems in delete in left-leaning red black trees:
    Tricky writing

    Sedgewick’s paper is tricky. As of 2013, the insert section presents 2–3–4 trees as the default and describes 2–3 trees as a variant. The delete implementation,
    however, only works for 2–3 trees. If you implement the default variant of insert and the only variant of delete, your tree won’t work. The text doesn’t highlight
    the switch from 2–3–4 to 2–3: not kind."


Other C++ implementations:
http://larryvdh.com/blog/red-black-tree-implementation/

http://users.cis.fiu.edu/~weiss/dsaa_c++/code/
