+ test.c should be retired to example.c and tests should be created with criterion,
   except its currently broken on gentoo so i will wait with that
+ a quoting configuration should be introduced that wraps strings and chars in ""/'',
   escapes quoting characters and converts control characters to something human readable
   (probably carot notation)
+ array printing; they dont play nice with generics and also: decay situation;
   the only reasonable thing to do is to allow them in formats
+ support for the most common library structs when a foreign header is detected
+ user type printing
+ maybe i should use nullptr\_t instead of my own null type
