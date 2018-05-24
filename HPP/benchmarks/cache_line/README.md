# Cache Lines

On going...


The idea here is to try and calculate the size of a cache line by using a
variable `step` size to iterate through an array. 
The idea is that a large enough `step` size will trigger a cache miss and we
will be able to see the changes in latency from having to retrieve another
cache line despite the fact that we are iterating through less elements.

Idea came from [Gallery of Processor Cache Effects](http://igoro.com/archive/gallery-of-processor-cache-effects/)
