####BBT test plan
-----------------

| Test ID       | Test Description | Initial Condition | Test Procedure | Expected Results |
| ------------- | ---------------- | ----------------- | -------------- | ---------------- |
|normal_use_empty|single time typical usage of the feature|empty pitcher (lid right side up), pitcher undocked,power on, software running|
|normal_use_full|single time typical usage of the feature|full pitcher (lid upside down), pitcher undocked, power on, software running|
|power_outage_standby|power outage during standby |no pitcher, power on, software running|
|power_outage_filling|power outage during filling|empty pitcher (lid right side up), picher undocked, power on, software running|
|power_outage_full|power outage when full|full pitcher (lid upside down), picher undocked, power on, software running|
|quick_replacement_during_filling(2)|quick pitcher replacement during filling state - repeat 2 times|empty pitcher (lid right side up), picher undocked, power on, software running|
|quick_replacement_during_filling(5)|quick pitcher replacement during filling state - repeat 5 times|empty pitcher (lid right side up), picher undocked, power on, software running|
|quick_replacement_when_full(2)|quick pitcher replacement during full state - repeat 2 times|full pitcher (lid upside down), pitcher undocked, power on, software running|
|quick_replacement_when_full(5)|quick pitcher replacement during full state - repeat 5 times|full pitcher (lid upside down), pitcher undocked, power on, software running|
|long_term_usage_light|extended light usage - one time per day|empty pitcher (lid right side up), picher undocked, power on, software running|
|long_term_usage_heavy|extended heavy usage - 10 times per day|empty pitcher (lid right side up), picher undocked, power on, software running|
|power_on_empty|initial power-up with pitcher installed and empty|empty pitcher (lid right side up), pitcher docked, power off, software not running|
|power_on_full|initial power-up with pitcher installed and full|full pitcher (lid upside down), pitcher docked, power off, software not running|
|power_on_no_pitcher|initial power-up with no pitcher|no pitcher lid, power off, software not running|
