<div class='scratchpads_vocabularies_info'>
	<table>
		<thead>
			<tr>
				<th>&nbsp;</th>
				<th><?php echo t('Name'); ?></th>
				<th><?php echo t('Description'); ?></th>
				<th><?php echo t('Version'); ?></th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<th><?php echo t('Source'); ?></th>
				<td><?php echo check_plain($vocabulary->name()); ?></td>
				<td><?php echo $vocabulary->description(); ?></td>
				<td><?php echo check_plain($vocabulary->version());?></td>
			</tr>
			<?php
			  $version = check_plain($receiver->version());
			  if(empty($version)){
                $version = t('(never synced)');
              }else{
                $count = count($receiver->pendingUpdates());
                  if($count){
                    $version .= '<br/><strong>' . t('(there are %count pending update(s) for the upgrade to this version to be complete)', array(
                      '%count' => $count
                    )) . '</strong>';
                  }
              }
            ?>
            <tr>
				<th><?php echo t('Destination'); ?></th>
				<td><?php echo check_plain($receiver->name()); ?></td>
				<td><?php echo $receiver->description(); ?></td>
				<td><?php echo $version;?></td>
			</tr>
		</tbody>
	</table>
</div>