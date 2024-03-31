using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExitCollider : MonoBehaviour
{
    private void OnTriggerEnter(Collider other) {
        if (GameManager.Instance.start) {
            if (other.CompareTag("Player")) {
                GameManager.Instance.winCondition += 1;
                other.gameObject.transform.parent.GetComponent<PlayerBehavior>().invisible = true;
            }
        }
    }
}
