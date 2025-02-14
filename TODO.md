+ mdictate is utterly broken;
   since each argument its own call it fails to keep track of where
   a line actually begins or ends
+ a quoting configuration should be introduced that wraps strings and chars in ""/'',
   escapes quoting characters and converts control characters to something human readable
   (probably carot notation)
+ array printing; they dont play nice with generics and also: decay situation;
   the only reasonable thing to do is to allow them in formats
+ support for the most common library structs when a foreign header is detected
+ user type printing
