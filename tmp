<< +    set cell minus 2 to 1
>> +++  set cell 0 to 3
> +++++ set cell 1 to 5
>> +    set cell 3 to 1
<<<     go to cell 0
[
  -     remove one to cell 0
  > -   remove one to cell 1
  [>]   goes to next zeroed cell (cell 2) if cell 1 is not 0
  <<    if cell 1 is not 0: go to cell 0
                      else: go to cell minus 1
]
<       go to cell minus 1 or minus 2
[>]     if on minus 2 go to minus 1
< -     set minus 2 to 0
>> #    dump cell 0
> #     dump cell 1