```mermaid
flowchart LR
   subgraph legend
       COMMENT[[comment]]
       LIBRARY[/library/]
       BINARY[/binary\]-- depends on -->LIBRARY[/library/]
   end

   subgraph A[clayknot]
   end

   subgraph B[clayknot-nodes]
   end

   B --> A
```
